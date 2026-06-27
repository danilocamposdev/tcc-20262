#pragma once
#include "../model/ProductionAllocation.h"
#include <vector>

class IProductionAllocationRepository {
public:
    virtual ~IProductionAllocationRepository() = default;
    virtual ProductionAllocation save(ProductionAllocation allocation) = 0;
    virtual std::vector<ProductionAllocation> all() const = 0;
    virtual std::vector<ProductionAllocation> by_order(int order_id) const = 0;
    virtual void clear() = 0;
};
