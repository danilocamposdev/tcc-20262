#pragma once
#include <optional>
#include "../model/Date.h"

class ProductionAllocation {
public:
    ProductionAllocation(int optimization_config_id, int order_id, Date date, int units)
        : optimization_config_id_{optimization_config_id},
          order_id_{order_id},
          date_{date},
          units_{units} {}

    std::optional<int> id()     const { return id_; }
    int optimization_config_id() const { return optimization_config_id_; }
    int order_id()              const { return order_id_; }
    Date date()                 const { return date_; }
    int units()                 const { return units_; }
    bool isPersisted()          const { return id_.has_value(); }
    void setId(int id)                { id_ = id; }

private:
    std::optional<int> id_;
    int optimization_config_id_;
    int order_id_;
    Date date_;
    int units_;
};
