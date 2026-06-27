#include "AllocationOptimizer.h"
#include <algorithm>

AllocationOptimizer::AllocationOptimizer(const ProblemData& data)
	: data_{data},
	result_status_{operations_research::sat::CpSolverStatus::UNKNOWN},
	orders_{data.order_repository().all()},
	molds_{data.mold_repository().all()},
	T_{data.horizon() + 1},
	start_date_{data.start_date()},
	daily_capacity_{data.daily_capacity()} {
		build_jobs();
	}

void AllocationOptimizer::build_jobs() {
	int job_id = 0;
	for (const Order& order : orders_) {
		int remaining = std::max(0, order.quantity() - order.produced_quantity());
		for (int u = 0; u < remaining; ++u) {
			jobs_.push_back({
					job_id++,
					order.id().value(),
					order.mold_id(),
					order.delivery_date().to_int(&start_date_),
					order.priority().value()
					});
		}
	}
}

void AllocationOptimizer::build_variables() {
	for (const Job& job : jobs_) {
		x_[job.id].resize(T_ + 1);
		for (int t = 1; t <= T_; ++t) {
			x_[job.id][t] = cp_model_.NewBoolVar().WithName(
					"x_" + std::to_string(job.id) + "_" + std::to_string(t));
		}
		for (int k = 1; k <= (T_ - job.deadline); ++k) {
			y_[job.id][k] = cp_model_.NewBoolVar().WithName(
					"y_" + std::to_string(job.id) + "_" + std::to_string(k));
		}
	}
}

void AllocationOptimizer::build_constraints() {
	using operations_research::sat::LinearExpr;

	// R1 - Every job must be produced exactly once
	for (const Job& job : jobs_) {
		LinearExpr sum_x_t;
		for (int t = 1; t <= T_; ++t)
			sum_x_t += x_[job.id][t];
		cp_model_.AddEquality(sum_x_t, 1);
	}

	// R3 - Daily capacity
	for (int t = 1; t < T_; ++t) {
		LinearExpr sum_x_j;
		for (const Job& job : jobs_)
			sum_x_j += x_[job.id][t];
		cp_model_.AddLessOrEqual(sum_x_j, daily_capacity_);
	}

	// R4 - Mold availability
	for (int t = 1; t < T_; ++t) {
		for (const Mold& mold : molds_) {
			if (!mold.isPersisted()) continue;
			int mid = mold.id().value();
			LinearExpr sum_x_j;
			for (const Job& job : jobs_)
				if (job.mold_id == mid)
					sum_x_j += x_[job.id][t];
			cp_model_.AddLessOrEqual(sum_x_j, mold.available_quantity());
		}
	}

	// R5 - Delay activation
	for (const Job& job : jobs_) {
		for (int k = 1; k <= (T_ - job.deadline); ++k) {
			LinearExpr activation = LinearExpr(y_[job.id][k]);
			int time_limit = job.deadline + k - 1;
			LinearExpr sum_x_t;
			for (int t = 1; t <= time_limit && t <= T_; ++t)
				sum_x_t += x_[job.id][t];
			activation += sum_x_t;
			cp_model_.AddEquality(activation, 1);
		}
	}
}

void AllocationOptimizer::build_objective_function() {
	using operations_research::sat::LinearExpr;

	const int artificial_day_penalty = 1000;
	const int earliness_weight = 1;
    const int delay_weight = (T_ * (int)jobs_.size()) + 1;

	LinearExpr sum_py_j_k;
	for (const Job& job : jobs_) {
		for (int k = 1; k <= (T_ - job.deadline); ++k) {
			int penalty = k * job.priority;
			if (job.deadline + k == T_)
				penalty += artificial_day_penalty * delay_weight;
			sum_py_j_k += LinearExpr::Term(y_[job.id][k], penalty);
		}
	}

	for (const Job& job : jobs_) {
		for (int t = 1; t <= T_; ++t) {
			sum_py_j_k += LinearExpr::Term(x_[job.id][t], t * earliness_weight);
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
	out << "    Daily capacity: " << daily_capacity_ << " unit(s)/day\n";
	out << "    Start date: " << start_date_.to_string() << "\n";
	out << "    Total jobs: " << jobs_.size() << "\n";

	out << "\n    Molds:\n";
	for (const Mold& mold : molds_) {
		out << "      [" << mold.id().value() << "] " << mold.type()
			<< " | available: " << mold.available_quantity() << "\n";
	}

	out << "\n    Orders:\n";
	for (const Order& order : orders_) {
		int remaining = std::max(0, order.quantity() - order.produced_quantity());
		out << "      [" << order.id().value() << "]"
			<< " | Deadline: " << order.delivery_date().to_string()
			<< " | Quantity: " << order.quantity()
			<< " | Produced: " << order.produced_quantity()
			<< " | Remaining: " << remaining
			<< " | Priority: " << order.priority().value() << "\n";
	}

	out << "\n  OPTIMAL ALLOCATION:\n\n";
	out << "    Z* = " << response_.objective_value() << "\n";

	for (int t = 1; t <= T_; ++t) {
		out << "\n    Day " << (t == T_
				? "T (artificial)"
				: std::to_string(t) + " (" + start_date_.sum_days(t - 1).to_string() + ")")
			<< ":\n";

		std::map<int, int> units_per_order;
		for (const Job& job : jobs_)
			if (operations_research::sat::SolutionBooleanValue(response_, x_[job.id][t]))
				units_per_order[job.order_id]++;

		if (units_per_order.empty()) {
			out << "      (No units scheduled)\n";
			continue;
		}

		for (const auto& [order_id, units] : units_per_order) {
			auto it = std::find_if(orders_.begin(), orders_.end(),
					[order_id](const Order& o) { return o.id().value() == order_id; });
			int deadline = it->delivery_date().to_int(&start_date_);
			out << "      Order [" << order_id << "]"
				<< " | units: " << units
				<< " | deadline: " << it->delivery_date().to_string();
			if (t != T_)
				out << (t > deadline
						? " [DELAYED by " + std::to_string(t - deadline) + " day(s)]"
						: " [ON TIME]");
			out << "\n";
		}
	}

	out << "\n" << separator << "\n";
}

void AllocationOptimizer::save_results(
    IOptimizationConfigRepository& config_repo,
    IProductionAllocationRepository& allocation_repo)
{
    // limpa resultados anteriores
    config_repo.clear();
    allocation_repo.clear();

    // salva a config atual e pega o id
    OptimizationConfig config{start_date_, T_ - 1, daily_capacity_};
    config = config_repo.save(config);

    // salva cada alocação
    for (int t = 1; t <= T_; ++t) {
        std::map<int, int> units_per_order;
        for (const Job& job : jobs_)
            if (operations_research::sat::SolutionBooleanValue(response_, x_[job.id][t]))
                units_per_order[job.order_id]++;

        Date date = start_date_.sum_days(t - 1);
        for (const auto& [order_id, units] : units_per_order) {
            ProductionAllocation allocation{config.id().value(), order_id, date, units};
            allocation_repo.save(allocation);
        }
    }
}
