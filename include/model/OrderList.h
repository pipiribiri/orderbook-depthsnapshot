//
// Created by marco on 07/12/2025.
//

#ifndef ORDERLIST_H
#define ORDERLIST_H
#include "InputOrderMessage.h"

class OrderList {
public:
    bool addNewOrder(const InputOrderMessage & newOrder, const int& depthLevels);
};

#endif //ORDERLIST_H
