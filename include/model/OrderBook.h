//
// Created by marco on 05/12/2025.
//

#ifndef ORDERBOOK_H
#define ORDERBOOK_H
#include <cstdint>

#include "InputOrderMessage.h"
#include "OrderList.h"


class OrderBook {
public:
    OrderBook(std::string  symbol, int depthLevels);

    void receiveNewOrder(const InputOrderMessage &newOrder, const uint32_t& sequenceNumber);

    const std::string& symbol() const { return m_symbol; }

private:
    std::string m_symbol;
    OrderList<std::greater<int64_t>> m_bids; // Bids ordered in descending order
    OrderList<std::less<int64_t>> m_asks;    // Asks ordered in ascending order
};

#endif //ORDERBOOK_H
