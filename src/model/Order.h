#pragma once
#include "Date.h"
#include <optional>
#include "Priority.h"

class Order {
	private:
		std::optional<int> id_;
		Date delivery_date_;
		int mold_id_;
		int client_id_;
		int quantity_;
		Priority priority_;
		int produced_quantity_;
	public:
		Order(Date delivery_date, int mold_id, int client_id, int quantity, Priority priority, int produced_quantity=0)
			: delivery_date_{delivery_date}, mold_id_{mold_id},
			client_id_{client_id}, quantity_{quantity}, priority_{priority},
			produced_quantity_{produced_quantity}{}

		std::optional<int> id() const { return id_; }
		void setId(int id) { id_ = id; }
		bool isPersisted() const { return id_.has_value(); }
		Date delivery_date() const { return delivery_date_; }
		int mold_id() const { return mold_id_; }
		int client_id() const { return client_id_; }
		int quantity() const { return quantity_; }
		Priority priority() const { return priority_; }
		int produced_quantity() const { return produced_quantity_; }
};
