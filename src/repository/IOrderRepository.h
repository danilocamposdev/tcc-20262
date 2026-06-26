#pragma once
#include "../model/Order.h"
#include <vector>
#include <optional>

class IOrderRepository {
public:
    virtual Order save(Order order) = 0;
    virtual std::optional<Order> get_by_id(int id) const = 0;
    virtual std::vector<Order> all() const = 0;
    virtual ~IOrderRepository() = default;
};
