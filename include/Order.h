//
// Created by marco on 07/12/2025.
//

#ifndef ORDER_H
#define ORDER_H
#include <cstdint>

#include "InputOrderMessage.h"

struct Order {
    OrderSide side;
    int64_t price;
    std::uint64_t volume;
};

#endif //ORDER_H
