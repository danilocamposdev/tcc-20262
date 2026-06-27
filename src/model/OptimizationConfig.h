#pragma once
#include <optional>
#include "../model/Date.h"

class OptimizationConfig {
public:
    OptimizationConfig(Date start_date, int horizon, int daily_capacity)
        : start_date_{start_date}, horizon_{horizon}, daily_capacity_{daily_capacity} {}

    std::optional<int> id()    const { return id_; }
    Date start_date()          const { return start_date_; }
    int horizon()              const { return horizon_; }
    int daily_capacity()       const { return daily_capacity_; }
    bool isPersisted()         const { return id_.has_value(); }
    void setId(int id)               { id_ = id; }

private:
    std::optional<int> id_;
    Date start_date_;
    int horizon_;
    int daily_capacity_;
};
