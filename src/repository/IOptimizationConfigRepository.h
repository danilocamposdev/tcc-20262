#pragma once
#include "../model/OptimizationConfig.h"
#include <optional>

class IOptimizationConfigRepository {
public:
    virtual ~IOptimizationConfigRepository() = default;
    virtual OptimizationConfig save(OptimizationConfig config) = 0;
    virtual std::optional<OptimizationConfig> current() const = 0;
    virtual void clear() = 0;
};
