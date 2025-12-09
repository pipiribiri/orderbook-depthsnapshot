//
// Created by marco on 05/12/2025.
//
#include <iostream>
#include <sstream>
#include <utility>

#include "../../include/model/OrderBook.h"

OrderBook::OrderBook(std::string symbol, const int depthLevels)
: m_symbol(std::move(symbol)),
  m_bids(depthLevels),
  m_asks(depthLevels)
{}

void OrderBook::receiveNewOrder(const InputOrderMessage &newOrder, const uint32_t& sequenceNumber)
{
    bool shouldProduceNewSnapshot = false;

    // First, add the order to the corresponding list
    if (newOrder.side == OrderSide::BUY)
    {
        shouldProduceNewSnapshot = m_bids.receiveNewOrder(newOrder);
    }
    else if (newOrder.side == OrderSide::SELL)
    {
        shouldProduceNewSnapshot = m_asks.receiveNewOrder(newOrder);
    }
    else
    {
        std::cerr << "Unrecognized order side: " << newOrder.side << std::endl;
    }

    if (shouldProduceNewSnapshot)
    {
        DepthSnapshot::publishSnapshot(
            sequenceNumber,
            m_symbol,
            m_bids.topLevels(),
            m_asks.topLevels());
    }
}
