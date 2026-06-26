#include "AllocationOptimizer.h"

AllocationOptimizer::AllocationOptimizer(const ProblemData& data)
    : data_{data},
      result_status_{operations_research::sat::CpSolverStatus::UNKNOWN},
      orders_{data.order_repository().all()},
      molds_{data.mold_repository().all()},
      T_{data.horizon() + 1},
      start_date_{data.start_date()},
      daily_capacity_{data.daily_capacity()} {}

void AllocationOptimizer::build_variables() {
    for (const Order& order : orders_) {
        int oid = order.id().value();
        x_[oid].resize(T_ + 1);
        for (int t = 1; t <= T_; ++t) {
            x_[oid][t] = cp_model_.NewBoolVar().WithName(
                "x_" + std::to_string(oid) + "_" + std::to_string(t));
        }
        for (int k = 1; k <= (T_ - order.delivery_date().to_int(&start_date_)); ++k) {
            y_[oid][k] = cp_model_.NewBoolVar().WithName(
                "y_" + std::to_string(oid) + "_" + std::to_string(k));
        }
    }
}

void AllocationOptimizer::build_constraints() {
    using operations_research::sat::LinearExpr;

    // R1 - Every order must be produced exactly once
    for (const Order& order : orders_) {
        int oid = order.id().value();
        LinearExpr sum_x_t;
        for (int t = 1; t <= T_; ++t)
            sum_x_t += x_[oid][t];
        cp_model_.AddEquality(sum_x_t, 1);
    }

    // R3 - Daily capacity (day T artificial has no limit)
    for (int t = 1; t < T_; ++t) {
        LinearExpr sum_x_j;
        for (const auto& order : orders_)
            sum_x_j += x_[order.id().value()][t];
        cp_model_.AddLessOrEqual(sum_x_j, daily_capacity_);
    }

    // R4 - Mold availability (artificial day T has no limit)
    for (int t = 1; t < T_; ++t) {
        for (const Mold& mold : molds_) {
            if (!mold.isPersisted()) continue;
            int mid = mold.id().value();
            LinearExpr sum_x_j;
            for (const auto& order : orders_)
                if (order.mold_id() == mid)
                    sum_x_j += x_[order.id().value()][t];
            cp_model_.AddLessOrEqual(sum_x_j, mold.available_quantity());
        }
    }

    // R5 - Delay activation
    for (const Order& order : orders_) {
        int oid = order.id().value();
        for (int k = 1; k <= (T_ - order.delivery_date().to_int(&start_date_)); ++k) {
            LinearExpr activation = LinearExpr(y_[oid][k]);
            int time_limit = order.delivery_date().to_int(&start_date_) + k - 1;
            LinearExpr sum_x_t;
            for (int t = 1; t <= time_limit && t <= T_; ++t)
                sum_x_t += x_[oid][t];
            activation += sum_x_t;
            cp_model_.AddEquality(activation, 1);
        }
    }
}

void AllocationOptimizer::build_objective_function() {
    using operations_research::sat::LinearExpr;

    const int artificial_day_penalty = 1000;

    LinearExpr sum_py_j_k;
    for (const Order& order : orders_) {
        int oid = order.id().value();
        for (int k = 1; k <= (T_ - order.delivery_date().to_int(&start_date_)); ++k) {
            int penalty = k * order.priority().value();
            if (order.delivery_date().to_int(&start_date_) + k == T_)
                penalty += artificial_day_penalty;
            sum_py_j_k += LinearExpr::Term(y_[oid][k], penalty);
        }
    }
    cp_model_.Minimize(sum_py_j_k);
}

bool AllocationOptimizer::solve() {
    build_variables();
    build_constraints();
    build_objective_function();

    operations_research::sat::Model model;
    response_ = operations_research::sat::SolveCpModel(cp_model_.Build(), &model);
    result_status_ = response_.status();

    if (result_status_ != operations_research::sat::CpSolverStatus::OPTIMAL &&
        result_status_ != operations_research::sat::CpSolverStatus::FEASIBLE) {
        std::cerr << "The problem does not have a feasible or optimal solution.\n";
        return false;
    }

    return true;
}

void AllocationOptimizer::print_results(std::ostream& out) {
    const std::string separator(72, '-');

    out << "\n" << separator << "\n";
    out << "  Order Allocation Plan\n";
    out << separator << "\n\n";

    out << "  PARAMETERS:\n\n";
    out << "    T: " << T_ - 1 << " working day(s) (+ 1 artificial)\n";
    out << "    Daily capacity: " << daily_capacity_ << " order(s)/day\n";
    out << "    Start date: " << start_date_.to_string() << "\n";

    out << "\n    Molds:\n";
    for (const Mold& mold : molds_) {
        out << "      [" << mold.id().value() << "] " << mold.type()
            << " | available: " << mold.available_quantity() << "\n";
    }

    out << "\n    Orders:\n";
    for (const Order& order : orders_) {
        out << "      [" << order.id().value() << "] "
            << " | Deadline: " << order.delivery_date().to_string()
            << " | Weight: " << order.priority().value() << "\n";
    }

    out << "\n  OPTIMAL ALLOCATION:\n\n";
    out << "    Z* = " << response_.objective_value() << "\n";

    for (int t = 1; t <= T_; ++t) {
        out << "\n    Day " << (t == T_
            ? "T (artificial)"
            : std::to_string(t) + " (" + start_date_.sum_days(t - 1).to_string() + ")")
            << ":\n";

        bool is_allocated = false;
        for (const Order& order : orders_) {
            int oid = order.id().value();
            if (operations_research::sat::SolutionBooleanValue(response_, x_[oid][t])) {
                int deadline = order.delivery_date().to_int(&start_date_);
                out << "      Order [" << oid << "]"
                    << " | deadline: " << order.delivery_date().to_string();
                if (t != T_)
                    out << (t > deadline
                        ? " [DELAYED by " + std::to_string(t - deadline) + " day(s)]"
                        : " [ON TIME]");
                out << "\n";
                is_allocated = true;
            }
        }
        if (!is_allocated) out << "    (No orders scheduled)\n";
    }

    out << "\n" << separator << "\n";
}
