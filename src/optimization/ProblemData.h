#pragma once
#include "../repository/IMoldRepository.h"
#include "../repository/IOrderRepository.h"
#include "../model/Date.h"

class ProblemData {
	private:
		Date start_date_;
		int horizon_;
		int daily_capacity_;
		IMoldRepository &mold_repository_;
		IOrderRepository &order_repository_;

	public:
		ProblemData(IMoldRepository &mold_repository, IOrderRepository &order_repository ,Date start_date, int horizon, int daily_capacity): 
			mold_repository_{mold_repository}, order_repository_{order_repository}, start_date_{start_date}, horizon_{horizon}, daily_capacity_{daily_capacity} {}

		

		Date start_date() const { return start_date_; }

		int horizon() const {return horizon_;}

		int daily_capacity() const {return daily_capacity_;}

		IMoldRepository& molds_repository() { return mold_repository_; }

		const IMoldRepository& molds_repository() const { return mold_repository_; }

		IOrderRepository& orders_repository() { return order_repository_; }

		const IOrderRepository& orders_repository() const { return order_repository_; }

};
