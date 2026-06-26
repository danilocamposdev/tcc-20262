#pragma once
#include <vector>
#include "../model/Order.h"

class IOrderRepository {
	public:
		virtual void add(Order order) = 0;
		virtual const Order* get_by_id(int id) const = 0;
		virtual const std::vector<Order>& all() const = 0;
		virtual ~IOrderRepository() = default;
};
