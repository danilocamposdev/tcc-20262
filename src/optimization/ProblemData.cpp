#include "ProblemData.h"

ProblemData::ProblemData(
		IMoldRepository *mold_repository,
		IOrderRepository *order_repository,
		Date start_date,
		int horizon,
		int daily_capacity)
	: start_date_{start_date},
	horizon_{horizon},
	daily_capacity_{daily_capacity},
	mold_repository_{mold_repository},
	order_repository_{order_repository} {}

	Date ProblemData::start_date() const { return start_date_; }
	int ProblemData::horizon() const { return horizon_; }
	int ProblemData::daily_capacity() const { return daily_capacity_; }

	IMoldRepository& ProblemData::mold_repository() { return *mold_repository_; }
	const IMoldRepository& ProblemData::mold_repository() const { return *mold_repository_; }

	IOrderRepository& ProblemData::order_repository() { return *order_repository_; }
	const IOrderRepository& ProblemData::order_repository() const { return *order_repository_; }
