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
               shouldUpdate = isPriceInTopLevel(price);
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

    void addOrder(uint64_t orderId, int64_t price, uint64_t volume)
    {
        m_orders[orderId] = {price, volume};
        auto [it, inserted] = m_book.emplace(price, 0);
        if (inserted) m_levelCount++;
        it->second += volume;
    }

    int64_t updateOrder(uint64_t orderId, int64_t newPrice, uint64_t newSize)
    {
        const auto oit = m_orders.find(orderId);
        if (oit == m_orders.end()) return invalidPrice();
        int64_t oldPrice = oit->second.price;
        uint64_t oldSize = oit->second.size;

        auto bit = m_book.find(oldPrice);
        if (bit != m_book.end()) {
            if (bit->second <= oldSize) { m_book.erase(bit); m_levelCount--; }
            else bit->second -= oldSize;
        }

        oit->second.price = newPrice;
        oit->second.size  = newSize;

        auto [nit, inserted] = m_book.emplace(newPrice, 0);
        if (inserted) m_levelCount++;
        nit->second += newSize;

        return newPrice;
    }

    int64_t executeOrder(uint64_t orderId, uint64_t tradedQuantity)
    {
        const auto oit = m_orders.find(orderId);
        if (oit == m_orders.end()) return invalidPrice();
        int64_t price = oit->second.price;

        auto bit = m_book.find(price);
        if (bit == m_book.end()) return invalidPrice();

        if (tradedQuantity >= oit->second.size) {
            tradedQuantity = oit->second.size;
            m_orders.erase(oit);
            if (bit->second <= tradedQuantity) { m_book.erase(bit); m_levelCount--; }
            else bit->second -= tradedQuantity;
        } else {
            oit->second.size -= tradedQuantity;
            bit->second -= tradedQuantity;
        }
        return price;
    }

    int64_t removeOrder(const uint64_t orderId)
    {
        const auto oit = m_orders.find(orderId);
        if (oit == m_orders.end()) return invalidPrice();
        int64_t price = oit->second.price;
        uint64_t size = oit->second.size;
        m_orders.erase(oit);

        auto bit = m_book.find(price);
        if (bit != m_book.end()) {
            if (bit->second <= size) { m_book.erase(bit); m_levelCount--; }
            else bit->second -= size;
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
        const auto it = m_orders.find(orderId);
        return it == m_orders.end() ? invalidPrice() : it->second.price;
    }

    bool levelsChanged()
    {
        bool changed = false;

        if (m_levelCount != m_lastLevelCount)
        {
            m_lastLevelCount = m_levelCount;
            changed = true;
        }
        return changed;
    }
};

#endif //ORDERLIST_H
