#pragma once
#include "../../src/optimization/problem_data.h"

inline ProblemData instance_01() {
	Date start_date = Date{2026,6,1};
	int horizon = 1;
	int daily_capacity = 1;

	ProblemData problem_data{start_date, horizon, daily_capacity};

	problem_data.molds_repository().add(Mold{0,"Matriz X", 1});

	problem_data.orders_repository().add(Order{0, Date{2026,6,1}, problem_data.molds_repository().get_by_id(0), true, 1});

	return problem_data;
}

inline ProblemData instance_02() {
	Date start_date = Date{2026,6,1};
	int horizon = 1;
	int daily_capacity = 1;

	ProblemData problem_data{start_date, horizon, daily_capacity};

	problem_data.molds_repository().add(Mold{0,"Matriz X", 1});

	problem_data.orders_repository().add(Order{0, Date{2026,6,1}, problem_data.molds_repository().get_by_id(0), false, 1});

	return problem_data;
}

inline ProblemData instance_03() {
	Date start_date = Date{2026,6,1};
	int horizon = 1;
	int daily_capacity = 1;

	ProblemData problem_data{start_date, horizon, daily_capacity};

	problem_data.molds_repository().add(Mold{0,"Matriz X", 1});
	problem_data.molds_repository().add(Mold{1,"Matriz Y", 1});

	problem_data.orders_repository().add(Order{0, Date{2026,6,1}, problem_data.molds_repository().get_by_id(0), true, 1});
	problem_data.orders_repository().add(Order{1, Date{2026,6,1}, problem_data.molds_repository().get_by_id(1), false, 1});

	return problem_data;
}

inline ProblemData instance_04() {
	Date start_date = Date{2026,6,1};
	int horizon = 1;
	int daily_capacity = 2;

	ProblemData problem_data{start_date, horizon, daily_capacity};

	problem_data.molds_repository().add(Mold{0,"Matriz X", 1});
	problem_data.molds_repository().add(Mold{1,"Matriz Y", 1});

	problem_data.orders_repository().add(Order{0, Date{2026,6,1}, problem_data.molds_repository().get_by_id(0), true, 1});
	problem_data.orders_repository().add(Order{1, Date{2026,6,1}, problem_data.molds_repository().get_by_id(1), false, 1});

	return problem_data;
}

inline ProblemData instance_05() {
	Date start_date = Date{2026,6,1};
	int horizon = 2;
	int daily_capacity = 1;

	ProblemData problem_data{start_date, horizon, daily_capacity};

	problem_data.molds_repository().add(Mold{0,"Matriz X", 1});
	problem_data.molds_repository().add(Mold{1,"Matriz Y", 1});

	problem_data.orders_repository().add(Order{0, Date{2026,6,1}, problem_data.molds_repository().get_by_id(0), false, 2});
	problem_data.orders_repository().add(Order{1, Date{2026,6,1}, problem_data.molds_repository().get_by_id(1), false, 3});
	problem_data.orders_repository().add(Order{2, Date{2026,6,1}, problem_data.molds_repository().get_by_id(1), false, 1});

	return problem_data;
}

inline ProblemData instance_06() {
	Date start_date = Date{2026,6,1};
	int horizon = 2;
	int daily_capacity = 2;

	ProblemData problem_data{start_date, horizon, daily_capacity};

	problem_data.molds_repository().add(Mold{0,"Matriz X", 1});

	problem_data.orders_repository().add(Order{0, Date{2026,6,1}, problem_data.molds_repository().get_by_id(0), false, 1});
	problem_data.orders_repository().add(Order{1, Date{2026,6,1}, problem_data.molds_repository().get_by_id(0), false, 1});

	return problem_data;
}

inline ProblemData instance_07() {
	Date start_date = Date{2026,6,1};
	int horizon = 5;
	int daily_capacity = 2;

	ProblemData problem_data{start_date, horizon, daily_capacity};

	problem_data.molds_repository().add(Mold{0,"Matriz X", 2});
	problem_data.molds_repository().add(Mold{1,"Matriz Y", 1});
	problem_data.molds_repository().add(Mold{2,"Matriz Z", 1});

	problem_data.orders_repository().add(Order{0, Date{2026,6,1}, problem_data.molds_repository().get_by_id(0), false, 1});
	problem_data.orders_repository().add(Order{1, Date{2026,6,1}, problem_data.molds_repository().get_by_id(0), false, 1});
	problem_data.orders_repository().add(Order{2, Date{2026,6,1}, problem_data.molds_repository().get_by_id(1), true, 1});
	problem_data.orders_repository().add(Order{3, Date{2026,6,2}, problem_data.molds_repository().get_by_id(2), false, 1});

	return problem_data;
}





