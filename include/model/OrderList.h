//
// Created by marco on 07/12/2025.
//

#ifndef ORDERLIST_H
#define ORDERLIST_H
#include <functional>
#include <limits>
#include <map>

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
   explicit OrderList(const int depthLevels) : m_levelCount(0), m_lastLevelCount(0), m_depthLevels(depthLevels) {}

   // Main function receiving a new order and choosing the operations to be made
   // based on its type and impact on the exising orders
   bool receiveNewOrder(const InputOrderMessage &newOrder)
   {
       bool shouldUpdateSnapshot = false;
       int64_t price;
       int64_t oldPrice;
       int64_t newPrice;

       switch (newOrder.type) {
           case OrderType::ADD:
               addOrder(newOrder.orderId, newOrder.price, newOrder.volume);
               shouldUpdateSnapshot = levelCountChanged() || isPriceInDepthLevel(newOrder.price);
               break;

           case OrderType::UPDATE:
               oldPrice = getOrderPrice(newOrder.orderId);
               newPrice = updateOrder(newOrder.orderId, newOrder.price, newOrder.volume);
               shouldUpdateSnapshot = levelCountChanged() || isPriceInDepthLevel(oldPrice) || isPriceInDepthLevel(newPrice);
               break;

           case OrderType::DELETE:
               price = removeOrder(newOrder.orderId);
               shouldUpdateSnapshot = levelCountChanged() || isPriceInDepthLevel(price);

           case OrderType::EXECUTE:
               price = executeOrder(newOrder.orderId, newOrder.tradedQuantity);
               shouldUpdateSnapshot = levelCountChanged() || isPriceInDepthLevel(price);
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

    // Number of levels for this order list, used in the depth calculation
    int m_levelCount;
    int m_lastLevelCount;

    // Depth level for the generated snapshots
    int m_depthLevels;

    // Ordered map keeping the order book, order is opposite for asks and bids
    std::map<int64_t, uint64_t, PriceOrder> m_book;

    // List of orders, used for order CRUD operations
    std::unordered_map<uint64_t, OrderRecord> m_orders;

    static constexpr int64_t invalidPrice() { return std::numeric_limits<int64_t>::min(); }

    void addOrder(const uint64_t orderId, int64_t price, uint64_t volume)
    {
        m_orders[orderId] = {price, volume};
        auto [it, inserted] = m_book.emplace(price, 0);
        it->second += volume;

        if (inserted)
        {
            recalculateLevelCount();
        }
    }

    int64_t updateOrder(const uint64_t orderId, int64_t newPrice, uint64_t newSize)
    {
        // First find the existing order with that ID
        int64_t price = invalidPrice();
        if (const auto orderIt = m_orders.find(orderId); orderIt != m_orders.end())
        {
            uint64_t oldSize = orderIt->second.size;

            // Then update it in the book and the order list with the new values
            // If an order is deleted or added, the level count needs to be updated
            if (auto bookIt = m_book.find(orderIt->second.price); bookIt != m_book.end())
            {
                if (bookIt->second <= oldSize)
                {
                    m_book.erase(bookIt);
                    recalculateLevelCount();
                }
                else bookIt->second -= oldSize;
            }
            orderIt->second.price = newPrice;
            orderIt->second.size  = newSize;

            if (auto [newOrderIt, inserted] = m_book.emplace(newPrice, newSize); inserted)
            {
                recalculateLevelCount();
            }
            price = newPrice;
        }
        return price;
    }

    int64_t executeOrder(const uint64_t orderId, uint64_t tradedQuantity)
    {
        int64_t price = invalidPrice();

        // Update the executed order in the book and the order list, if it is erased, update level count for the snapshot
        if (const auto orderIt = m_orders.find(orderId); orderIt != m_orders.end())
        {
            if (auto bookIt = m_book.find(orderIt->second.price); bookIt != m_book.end())
            {
                price = orderIt->second.price;

                if (tradedQuantity >= orderIt->second.size) {
                    tradedQuantity = orderIt->second.size;
                    m_orders.erase(orderIt);
                    if (bookIt->second <= tradedQuantity)
                    {
                        m_book.erase(bookIt);
                        recalculateLevelCount();
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
        return price;
    }

    int64_t removeOrder(const uint64_t orderId)
    {
        int64_t price = invalidPrice();

        // First remove it from the orders
        if (const auto orderIt = m_orders.find(orderId); orderIt != m_orders.end())
        {
            price = orderIt->second.price;

            // Then remove it from the book
            if (auto bookIt = m_book.find(price); bookIt != m_book.end())
            {
                // Delete it completely or partially, updating the level count
                if (bookIt->second <= orderIt->second.size)
                {
                    m_book.erase(bookIt);
                    recalculateLevelCount();
                }
                else bookIt->second -= orderIt->second.size;
            }
        }

        return price;
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

    int64_t getOrderPrice(const uint64_t orderId) const
    {
        int64_t ret = invalidPrice();

        if (const auto it = m_orders.find(orderId); it != m_orders.end())
        {
            ret = it->second.price;
        }

        return ret;
    }

    void recalculateLevelCount()
    {
        // Recalculate level count, the maximum value is the depth level
        m_levelCount = 0;
        int count = 0;

        for (auto it2 = m_book.begin(); it2 != m_book.end() && count < m_depthLevels; ++it2, ++count)
        {
            if (it2->second > 0)
            {
                m_levelCount++;
            }
        }
    }

    // Checks if the level count changed in the last operation
    bool levelCountChanged()
    {
        const bool changed = m_levelCount != m_lastLevelCount;

        if (changed)
        {
            m_lastLevelCount = m_levelCount;
        }
        return changed;
    }
};

#endif //ORDERLIST_H
