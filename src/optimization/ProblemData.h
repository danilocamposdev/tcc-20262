#pragma once
#include "../repository/IMoldRepository.h"
#include "../repository/IOrderRepository.h"
#include "../model/Date.h"

class ProblemData {
private:
    Date start_date_;
    int horizon_;
    int daily_capacity_;
    IMoldRepository *mold_repository_;
    IOrderRepository *order_repository_;

public:
    ProblemData(
        IMoldRepository *mold_repository,
        IOrderRepository *order_repository,
        Date start_date,
        int horizon,
        int daily_capacity);

    Date start_date() const;
    int horizon() const;
    int daily_capacity() const;
    IMoldRepository& mold_repository();
    const IMoldRepository& mold_repository() const;
    IOrderRepository& order_repository();
    const IOrderRepository& order_repository() const;
};
