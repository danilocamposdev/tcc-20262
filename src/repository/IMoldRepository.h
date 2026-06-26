#pragma once
#include <vector>
#include "../model/Mold.h"

class IMoldRepository {
	public:
		virtual void add(Mold mold) = 0;

		virtual const Mold* get_by_id(int id) const = 0;

		virtual const std::vector<Mold>& all() const = 0;

		virtual ~IMoldRepository() = default;
};
