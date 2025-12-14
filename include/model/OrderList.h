//
// Created by marco on 07/12/2025.
//

#ifndef ORDERLIST_H
#define ORDERLIST_H
#include <functional>
#include <limits>
#include <map>
#include <iostream>

#include "DepthSnapshot.h"
#include "InputOrderMessage.h"

struct OrderRecord {
    int64_t price;
    uint64_t size;
};

// Templated order list with compile-time comparator
template <typename PriceOrder>
class OrderList {
public:
   explicit OrderList(const int depthLevels) : m_depthLevels(depthLevels) {}

   // Main function receiving a new order and choosing the operations to be made
   // based on its type and impact on the exising orders
   bool receiveNewOrder(const InputOrderMessage &newOrder)
   {
       bool shouldUpdateSnapshot = false;

       switch (newOrder.type) {
           case OrderType::ADD:
               shouldUpdateSnapshot = addOrder(newOrder.orderId, newOrder.price, newOrder.volume);
               break;

           case OrderType::UPDATE:
               shouldUpdateSnapshot = updateOrder(newOrder.orderId, newOrder.price, newOrder.volume);
               break;

           case OrderType::DELETE:
               shouldUpdateSnapshot = removeOrder(newOrder.orderId);
               break;

           case OrderType::EXECUTE:
               shouldUpdateSnapshot = executeOrder(newOrder.orderId, newOrder.tradedQuantity);
               break;
       }
       return shouldUpdateSnapshot;
   }

    std::vector<DepthLevel> topLevels() const
    {
        std::vector<DepthLevel> levels;
        if (m_depthLevels <= 0 || m_book.empty())
        {
            return levels;
        }
        levels.reserve(m_depthLevels);

        int count = 0;
        for (auto it = m_book.begin(); it != m_book.end() && count < m_depthLevels; ++it, ++count) {
            levels.push_back({it->first, it->second});
        }
        return levels;
    }

private:
    // Depth level for the generated snapshots
    int m_depthLevels;

    // Ordered map keeping the order book, order is opposite for asks and bids
    std::map<int64_t, uint64_t, PriceOrder> m_book;

    // List of orders, used for order CRUD operations
    std::unordered_map<uint64_t, OrderRecord> m_orders;

    static constexpr int64_t invalidPrice() { return std::numeric_limits<int64_t>::min(); }

    bool addOrder(const uint64_t orderId, int64_t price, uint64_t volume)
    {
        bool shouldUpdateSnapshot = false;

        m_orders[orderId] = {price, volume};
        auto [it, inserted] = m_book.emplace(price, 0);
        it->second += volume;

        if (inserted && isPriceInDepthLevel(price))
        {
            shouldUpdateSnapshot = true;
        }

        return shouldUpdateSnapshot;
    }

    bool updateOrder(const uint64_t orderId, int64_t newPrice, uint64_t newSize)
    {
        bool shouldUpdateSnapshot = false;

        // First find the existing order with that ID
        if (const auto orderIt = m_orders.find(orderId); orderIt != m_orders.end())
        {
            int64_t oldPrice = orderIt->second.price;
            uint64_t oldSize = orderIt->second.size;

            // Then update it in the book and the order list with the new values
            // If an order is deleted or added, the level count needs to be updated
            shouldUpdateSnapshot = isPriceInDepthLevel(oldPrice) || isPriceInDepthLevel(newPrice);

            if (auto bookIt = m_book.find(orderIt->second.price); bookIt != m_book.end())
            {

                if (bookIt->second <= oldSize)
                {
                    m_book.erase(bookIt);
                }
                else bookIt->second -= oldSize;
            }
            orderIt->second.price = newPrice;
            orderIt->second.size  = newSize;

            if (auto [newOrderIt, inserted] = m_book.emplace(newPrice, newSize); !inserted)
            {
                std::cout << "Error updating order with ID " << orderId << std::endl;
            }
        }
        return shouldUpdateSnapshot;
    }

    bool executeOrder(const uint64_t orderId, uint64_t tradedQuantity)
    {
        bool shouldUpdateSnapshot = false;

        // Update the executed order in the book and the order list, if it is erased, update level count for the snapshot
        if (const auto orderIt = m_orders.find(orderId); orderIt != m_orders.end())
        {
            if (auto bookIt = m_book.find(orderIt->second.price); bookIt != m_book.end())
            {
                // Update snapshot if needed - Price in depth level and difference in volume
                shouldUpdateSnapshot = isPriceInDepthLevel(orderIt->second.price) && tradedQuantity != 0;

                if (tradedQuantity >= orderIt->second.size) {
                    tradedQuantity = orderIt->second.size;
                    m_orders.erase(orderIt);
                    if (bookIt->second <= tradedQuantity)
                    {
                        m_book.erase(bookIt);
                    }
                    else
                    {
                        bookIt->second -= tradedQuantity;
                    }
                }
                else
                {
                    orderIt->second.size -= tradedQuantity;
                    bookIt->second -= tradedQuantity;
                }
            }
        }
        return shouldUpdateSnapshot;
    }

    bool removeOrder(const uint64_t orderId)
    {
        bool shouldUpdateSnapshot = false;

        // First remove it from the orders
        if (const auto orderIt = m_orders.find(orderId); orderIt != m_orders.end())
        {
            int64_t price = orderIt->second.price;

            // Then remove it from the book
            if (auto bookIt = m_book.find(price); bookIt != m_book.end())
            {
                // Delete it completely or partially, updating the level count
                if (bookIt->second <= orderIt->second.size)
                {
                    shouldUpdateSnapshot = isPriceInDepthLevel(price);
                    m_book.erase(bookIt);
                }
                else bookIt->second -= orderIt->second.size;
            }
        }

        return shouldUpdateSnapshot;
    }

    // Checks if the price of an order is inside the depth level
    bool isPriceInDepthLevel(int64_t price) const
    {
        int count = 0;
        for (auto it = m_book.begin(); it != m_book.end() && count < m_depthLevels; ++it, ++count)
        {
            if (it->first == price) return true;
        }
        return false;
    }
};

#endif //ORDERLIST_H
