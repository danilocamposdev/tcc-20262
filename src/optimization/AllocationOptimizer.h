#pragma once
#include <iostream>
#include <map>
#include <vector>
#include "ortools/sat/cp_model.h"
#include "./ProblemData.h"

class AllocationOptimizer {
	private:
		const ProblemData& data_;
		operations_research::sat::CpSolverStatus result_status_;
		operations_research::sat::CpModelBuilder cp_model_;
		operations_research::sat::CpSolverResponse response_;

		std::map<int, std::vector<operations_research::sat::BoolVar>> x_;
		std::map<int, std::map<int, operations_research::sat::BoolVar>> y_;
		const std::vector<Order>& orders_;
		const std::vector<Mold>&  molds_;
		const int T_;
		const Date start_date_;
		const int daily_capacity_;

		void build_variables() {
			for (const Order& order : orders_) {
				x_[order.id()].resize(T_ + 1);
				for (int t = 1; t <= T_; ++t) {
					x_[order.id()][t] = cp_model_.NewBoolVar().WithName(
							"x_" + std::to_string(order.id()) + "_" + std::to_string(t));
				}
				for (int k = 1; k <= (T_ - order.delivery_date().to_int(&start_date_)); ++k) {
					y_[order.id()][k] = cp_model_.NewBoolVar().WithName(
							"y_" + std::to_string(order.id()) + "_" + std::to_string(k));
				}
			}
		}

		void build_constraints() {
			using operations_research::sat::LinearExpr;

			// R1 - Every regular order must be produced exactly once
			for (const Order& order : orders_) {
				if (order.is_firm()) continue;
				LinearExpr sum_x_t;
				for (int t = 1; t <= T_; ++t)
					sum_x_t += x_[order.id()][t];
				cp_model_.AddEquality(sum_x_t, 1);
			}

			// R2 - Firm orders delivered must be produced within deadline
			for (const Order& order : orders_) {
				if (!order.is_firm()) continue;
				LinearExpr sum_x_t;
				for (int t = 1; t <= order.delivery_date().to_int(&start_date_); ++t)
					sum_x_t += x_[order.id()][t];
				cp_model_.AddEquality(sum_x_t, 1);
			}

			// R3 - Daily capacity (day T artificial has no limit)
			for (int t = 1; t < T_; ++t) {
				LinearExpr sum_x_j;
				for (const auto& order : orders_) sum_x_j += x_[order.id()][t];
				cp_model_.AddLessOrEqual(sum_x_j, daily_capacity_);
			}

			// R4 - Mold availability (artificial day T has no limit)
			for (int t = 1; t < T_; ++t) {
				for (const Mold& mold : molds_) {
					LinearExpr sum_x_j;
					for (const auto& order : orders_)
						if (order.mold() == &mold) sum_x_j += x_[order.id()][t];
					cp_model_.AddLessOrEqual(sum_x_j, mold.available_quantity());
				}
			}

			// R5 - Delay activation
			for (const Order& order : orders_) {
				for (int k = 1; k <= (T_ - order.delivery_date().to_int(&start_date_)); ++k) {
					if (order.is_firm()) continue;
					LinearExpr activation = LinearExpr(y_[order.id()][k]);
					int time_limit = order.delivery_date().to_int(&start_date_) + k - 1;
					LinearExpr sum_x_t;
					for (int t = 1; t <= time_limit && t <= T_; ++t)
						sum_x_t += x_[order.id()][t];
					activation += sum_x_t;
					cp_model_.AddEquality(activation, 1);
				}
			}
		}

		void build_objective_function() {
			using operations_research::sat::LinearExpr;

			const int artificial_day_penalty = 1000;

			LinearExpr sum_py_j_k;
			for (const Order& order : orders_) {
				if (order.is_firm()) continue;
				for (int k = 1; k <= (T_ - order.delivery_date().to_int(&start_date_)); ++k) {
					int penalty = k * order.weight();

					// se k corresponde ao dia T, adiciona penalidade extra
					if (order.delivery_date().to_int(&start_date_) + k == T_) {
						penalty += artificial_day_penalty;
					}

					sum_py_j_k += LinearExpr::Term(y_[order.id()][k], penalty);
				}
			}
			cp_model_.Minimize(sum_py_j_k);
		}

	public:
		AllocationOptimizer(const ProblemData& data)
			: data_{data},
			result_status_{operations_research::sat::CpSolverStatus::UNKNOWN},
			orders_{data.order_repository().all()},
			molds_{data.mold_repository().all()},
			T_{data.horizon() + 1},
			start_date_{data.start_date()},
			daily_capacity_{data.daily_capacity()} {}

		bool solve() {
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

		void print_results(std::ostream& out = std::cout) {
			const std::string separator(72, '-');

			out << "\n" << separator << "\n";
			out << "  Order Allocation Plan — Barbosa Minerals\n";
			out << separator << "\n\n";

			out << "  PARAMETERS:\n\n";
			out << "    T: " << T_ - 1 << " working day(s) (+ 1 artificial)\n";
			out << "    Daily capacity: " << daily_capacity_ << " order(s)/day\n";
			out << "    Start date: " << start_date_.to_string() << "\n";

			out << "\n    Molds:\n";
			for (const Mold& mold : molds_) {
				out << "      [" << mold.id() << "] " << mold.type()
					<< " | available: " << mold.available_quantity() << "\n";
			}

			out << "\n    Orders:\n";
			for (const Order& order : orders_) {
				out << "      [" << order.id() << "] "
					<< "Mold: " << order.mold()->type()
					<< " | Deadline: " << order.delivery_date().to_string()
					<< " | Firm: " << (order.is_firm() ? "yes" : "no")
					<< " | Weight: " << order.weight() << "\n";
			}

			out << "\n  OPTIMAL ALLOCATION:\n\n";
			out << "    Z* (Minimized Total Penalty) = "
				<< response_.objective_value()<< "\n";

			for (int t = 1; t <= T_; ++t) {
				out << "\n    Day " << (t == T_
						? "T (artificial)"
						: std::to_string(t) + " (" + start_date_.sum_days(t - 1).to_string() + ")")
					<< ":\n";

				bool is_allocated = false;
				for (const Order& order : orders_) {
					if (operations_research::sat::SolutionBooleanValue(response_, x_[order.id()][t])) {
						int deadline = order.delivery_date().to_int(&start_date_);
						out << "      Order [" << order.id() << "]"
							<< " mold: " << order.mold()->type()
							<< " | deadline: " << order.delivery_date().to_string()
							<< " | firm: " << (order.is_firm() ? "yes" : "no");

						if (t != T_) {
							out << (t > deadline
									? " [DELAYED by " + std::to_string(t - deadline) + " day(s)]"
									: " [ON TIME]");
						}
						out << "\n";
						is_allocated = true;
					}
				}
				if (!is_allocated) out << "    (No orders scheduled)\n";
			}

			out << "\n" << separator << "\n";
		}
};
