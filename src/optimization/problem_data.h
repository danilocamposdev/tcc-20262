#pragma once
#include "../repository/mold_repository.h"
#include "../repository/order_repository.h"
#include "../model/date.h"

class ProblemData {
	private:
		Date start_date_;
		int horizon_;
		int daily_capacity_;
		MoldRepository molds_repository_;
		OrderRepository orders_repository_;

	public:
		ProblemData(Date start_date, int horizon, int daily_capacity): 
			start_date_{start_date}, horizon_{horizon}, daily_capacity_{daily_capacity} {}

		Date start_date() const { return start_date_; }

		int horizon() const {return horizon_;}

		int daily_capacity() const {return daily_capacity_;}

		MoldRepository& molds_repository() { return molds_repository_; }

		const MoldRepository& molds_repository() const { return molds_repository_; }

		OrderRepository& orders_repository() { return orders_repository_; }

		const OrderRepository& orders_repository() const { return orders_repository_; }

};
