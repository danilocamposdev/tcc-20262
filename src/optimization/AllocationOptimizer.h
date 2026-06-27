#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "ortools/sat/cp_model.h"
#include "ProblemData.h"
#include "../model/Order.h"
#include "../model/Mold.h"
#include "../repository/IOptimizationConfigRepository.h"
#include "../repository/IProductionAllocationRepository.h"

struct Job {
	int id;
	int order_id;
	int mold_id;
	int deadline;
	int priority;
};

class AllocationOptimizer {
	private:
		const ProblemData& data_;
		operations_research::sat::CpSolverStatus result_status_;
		operations_research::sat::CpModelBuilder cp_model_;
		operations_research::sat::CpSolverResponse response_;

		std::map<int, std::vector<operations_research::sat::BoolVar>> x_;
		std::map<int, std::map<int, operations_research::sat::BoolVar>> y_;
		const std::vector<Order> orders_;
		const std::vector<Mold>  molds_;
		std::vector<Job> jobs_;
		const int T_;
		const Date start_date_;
		const int daily_capacity_;

		void build_jobs();
		void build_variables();
		void build_constraints();
		void build_objective_function();

	public:
		AllocationOptimizer(const ProblemData& data);
		bool solve();
		void print_results(std::ostream& out = std::cout);
		void save_results(IOptimizationConfigRepository& config_repo,
				IProductionAllocationRepository& allocation_repo
				);

};
