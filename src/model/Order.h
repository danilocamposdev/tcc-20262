#pragma once
#include "Date.h"

class Order {
	private:
		int id_;
		Date delivery_date_;
		int mold_id_;
		int client_id_;
		int quantity_;
		int priority_;
	public:
		Order(int id, Date delivery_date, int mold_id, int client_id, int quantity, int priority)
			: id_{id}, delivery_date_{delivery_date}, mold_id_{mold_id},
			client_id_{client_id}, quantity_{quantity}, priority_{priority} {}

		int id() const { return id_; }
		Date delivery_date() const { return delivery_date_; }
		int mold_id() const { return mold_id_; }
		int client_id() const { return client_id_; }
		int quantity() const { return quantity_; }
		int priority() const { return priority_; }
};
