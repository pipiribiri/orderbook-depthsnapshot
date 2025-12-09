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
    explicit OrderList(int depthLevels) : m_levelCount(0), m_lastLevelCount(0), m_depthLevels(depthLevels) {}

   bool receiveNewOrder(const InputOrderMessage &newOrder)
   {
       bool shouldUpdate = false;
       int64_t price;
       int64_t oldPrice;
       int64_t newPrice;

       switch (newOrder.type) {
           case OrderType::ADD:
               addOrder(newOrder.orderId, newOrder.price, newOrder.volume);
               shouldUpdate = levelsChanged() || isPriceInTopLevel(newOrder.price);
               break;

           case OrderType::UPDATE:
               oldPrice = getOrderPrice(newOrder.orderId);
               newPrice = updateOrder(newOrder.orderId, newOrder.price, newOrder.volume);
               shouldUpdate = levelsChanged() || isPriceInTopLevel(oldPrice) || isPriceInTopLevel(newPrice);
               break;

           case OrderType::DELETE:
               price = removeOrder(newOrder.orderId);
               shouldUpdate = levelsChanged() || isPriceInTopLevel(price);

           case OrderType::EXECUTE:
               price = executeOrder(newOrder.orderId, newOrder.tradedQuantity);
               shouldUpdate = levelsChanged() || isPriceInTopLevel(price);
               break;
       }
       return shouldUpdate;
   }

    std::vector<DepthLevel> topLevels() const
    {
        std::vector<DepthLevel> levels;
        if (m_depthLevels <= 0 || m_book.empty()) return levels;
        levels.reserve(m_depthLevels);
        int count = 0;
        for (auto it = m_book.begin(); it != m_book.end() && count < m_depthLevels; ++it, ++count) {
            levels.push_back({it->first, it->second});
        }
        return levels;
    }

private:
    int m_levelCount;
    int m_lastLevelCount;
    int m_depthLevels;
    std::map<int64_t, uint64_t, PriceOrder> m_book;
    std::unordered_map<uint64_t, OrderRecord> m_orders;

    static constexpr int64_t invalidPrice() { return std::numeric_limits<int64_t>::min(); }

    void addOrder(const uint64_t orderId, int64_t price, uint64_t volume)
    {
        m_orders[orderId] = {price, volume};
        auto [it, inserted] = m_book.emplace(price, 0);
        if (inserted) m_levelCount++;
        it->second += volume;
    }

    int64_t updateOrder(const uint64_t orderId, int64_t newPrice, uint64_t newSize)
    {
        int64_t price = invalidPrice();
        if (const auto orderIt = m_orders.find(orderId); orderIt != m_orders.end())
        {
            uint64_t oldSize = orderIt->second.size;

            if (auto bookIt = m_book.find(orderIt->second.price); bookIt != m_book.end()) {
                if (bookIt->second <= oldSize)
                {
                    m_book.erase(bookIt);
                    m_levelCount--;
                }
                else bookIt->second -= oldSize;
            }
            orderIt->second.price = newPrice;
            orderIt->second.size  = newSize;

            auto [newOrderIt, inserted] = m_book.emplace(newPrice, 0);
            if (inserted)
            {
                m_levelCount++;
            }
            newOrderIt->second += newSize;

            price = newPrice;
        }
        return price;
    }

    int64_t executeOrder(const uint64_t orderId, uint64_t tradedQuantity)
    {
        int64_t price = invalidPrice();

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
                        m_levelCount--;
                    }
                    else bookIt->second -= tradedQuantity;
                } else {
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
            if (auto bookIt = m_book.find(price); bookIt != m_book.end()) {
                // Delete it completely or partially
                if (bookIt->second <= orderIt->second.size) { m_book.erase(bookIt); m_levelCount--; }
                else bookIt->second -= orderIt->second.size;
            }
        }

        return price;
    }

    bool isPriceInTopLevel(int64_t price) const
    {
        int count = 0;
        for (auto it = m_book.begin(); it != m_book.end() && count < m_depthLevels; ++it, ++count) {
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

    bool levelsChanged()
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
