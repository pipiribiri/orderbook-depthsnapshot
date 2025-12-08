//
// Created by marco on 05/12/2025.
//
#include <iostream>
#include <utility>

#include "../../include/model/DepthSnapshot.h"

#include <sstream>

DepthSnapshot::DepthSnapshot(std::string symbol, const int& depthLevels)
: m_depthLevels(depthLevels),
  m_symbol(std::move(symbol))
{}

void DepthSnapshot::addNewOrder(const InputOrderMessage &newOrder, const uint32_t& sequenceNumber)
{
    bool shouldProduceNewSnapshot = false;

    // First, add the order to the corresponding list
    if (newOrder.side == OrderSide::BUY)
    {
        shouldProduceNewSnapshot = m_bids.addNewOrder(newOrder, m_depthLevels);
    }
    else if (newOrder.side == OrderSide::SELL)
    {
        shouldProduceNewSnapshot = m_asks.addNewOrder(newOrder, m_depthLevels);
    }
    else
    {
        std::cerr << "Unrecognized order side: " << newOrder.side << std::endl;
    }

    // Then produce a new snapshot if applicable
    if (shouldProduceNewSnapshot)
    {
        publishSnapshot(sequenceNumber);
    }
}

void DepthSnapshot::publishSnapshot(const uint32_t& sequenceNumber)
{
    // DepthSnapshot snap;
    int count = 0;
    // for (auto &[price, vol]: m_bids) {
    //     if (count >= m_depthLevels) break;
    //     snap.bids.push_back({price, vol});
    //     count++;
    // }
    //
    // count = 0;
    // for (auto &[price, vol]: m_asks) {
    //     if (count >= m_depthLevels) break;
    //     snap.asks.push_back({price, vol});
    //     count++;
    // }
    // if (snap != m_lastSnapshot) {
    //     std::cout << snap.toString(sequenceNo, m_symbol) << "\n";
    //     m_lastSnapshot = snap;
    // }

    // std::ostringstream oss;
    // oss << m_sequenceNumber << ", " << m_symbol << ", [";
    // for (size_t i = 0; i < m_bids.size(); ++i) {
    //     if (i > 0) oss << ", ";
    //     oss << "(" << m_bids[i].price << ", " << m_bids[i].volume << ")";
    // }
    // oss << "], [";
    // for (size_t i = 0; i < asks.size(); ++i) {
    //     if (i > 0) oss << ", ";
    //     oss << "(" << m_asks[i].price << ", " << m_asks[i].volume << ")";
    // }
    // oss << "]";
    // return oss.str();
}

// #include <sstream>
//
//
// bool DepthSnapshot::operator==(const DepthSnapshot &other) const
// {
//     return bids == other.bids && asks == other.asks;
// }
//
// std::string DepthSnapshot::toString(uint32_t sequenceNo, const std::string &symbol) const
// {
//     std::ostringstream oss;
//     oss << sequenceNo << ", " << symbol << ", [";
//     for (size_t i = 0; i < bids.size(); ++i) {
//         if (i > 0) oss << ", ";
//         oss << "(" << bids[i].price << ", " << bids[i].volume << ")";
//     }
//     oss << "], [";
//     for (size_t i = 0; i < asks.size(); ++i) {
//         if (i > 0) oss << ", ";
//         oss << "(" << asks[i].price << ", " << asks[i].volume << ")";
//     }
//     oss << "]";
//     return oss.str();
// }

//
//
//
// void DepthSnapshotGenerator::handleOrderUpdate(const InputOrderMessage &msg, uint32_t sequenceNo, int depthLevels)
// {
//     switch (msg.type) {
//         case OrderType::ADD: addOrder(msg);
//             break;
//         case OrderType::UPDATE: updateOrder(msg);
//             break;
//         case OrderType::DELETE: deleteOrder(msg);
//             break;
//         case OrderType::EXECUTE: executeOrder(msg);
//             break;
//     }
//
//     if (shouldUpdateSnapshot())
//     {
//         publishSnapshot(sequenceNo, depthLevels);
//     }
// }
//
// std::map<int64_t, uint64_t> DepthSnapshotGenerator::selectOrderBookSide(const OrderSide &orderSide)
// {
//     if (orderSide == OrderSide::BUY) {
//         return m_bids;
//     } else {
//         return m_asks;
//     }
// }
//
// void DepthSnapshotGenerator::addOrder(const InputOrderMessage &msg)
// {
//     // Add order to list of orders
//     m_orders[msg.orderId] = {msg.side, msg.price, msg.volume};
//
//     // Update Order book
//     if (msg.side == OrderSide::BUY) {}
//     std::map<int64_t, uint64_t>& book = selectOrderBookSide(msg.side);
//     book[msg.price] += msg.volume;
// }
//
// void DepthSnapshotGenerator::updateOrder(const InputOrderMessage &msg)
// {
//     auto it = m_orders.find(msg.orderId);
//
//     if (it == m_orders.end()) {
//         std::cout << "Invalid order update, order doesn't exist" << std::endl;
//     } else {
//         auto &existingOrder = it->second;
//         auto &book = selectOrderBookSide(existingOrder.side);
//
//         //
//         book[existingOrder.price] -= existingOrder.volume;
//
//         if (book[existingOrder.price] == 0) book.erase(existingOrder.price);
//
//         existingOrder.price = msg.price;
//         existingOrder.volume = msg.volume;
//         auto &newBook = selectOrderBookSide(existingOrder.side);
//         newBook[existingOrder.price] += existingOrder.volume;
//     }
// }
//
// void DepthSnapshotGenerator::deleteOrder(const InputOrderMessage &msg)
// {
//     auto it = m_orders.find(msg.orderId);
//
//     if (it == m_orders.end()) {
//         std::cout << "Invalid order deletion, order doesn't exist" << std::endl;
//     } else {
//         auto &existingOrder = it->second;
//         auto &book = selectOrderBookSide(existingOrder.side);
//
//         book[existingOrder.price] -= existingOrder.volume;
//
//         if (book[existingOrder.price] == 0) book.erase(existingOrder.price);
//
//         m_orders.erase(it);
//     }
// }
//
// void DepthSnapshotGenerator::executeOrder(const InputOrderMessage &msg)
// {
//     auto it = m_orders.find(msg.orderId);
//
//     if (it == m_orders.end()) {
//         std::cout << "Invalid order execution, order doesn't exist" << std::endl;
//     } else {
//         auto &existingOrder = it->second;
//         auto &book = selectOrderBookSide(existingOrder.side);
//
//         if (msg.tradedQuantity >= existingOrder.volume) {
//             book[existingOrder.price] -= existingOrder.volume;
//             if (book[existingOrder.price] == 0) book.erase(existingOrder.price);
//             m_orders.erase(it);
//         } else {
//             existingOrder.volume -= msg.tradedQuantity;
//             book[existingOrder.price] -= msg.tradedQuantity;
//         }
//     }
// }
//
// void DepthSnapshotGenerator::publishSnapshot(uint32_t sequenceNo, int depthLevels)
// {
//     DepthSnapshot snap;
//     int count = 0;
//     for (auto &[price, vol]: m_bids) {
//         if (count >= depthLevels) break;
//         snap.bids.push_back({price, vol});
//         count++;
//     }
//
//     count = 0;
//     for (auto &[price, vol]: m_asks) {
//         if (count >= depthLevels) break;
//         snap.asks.push_back({price, vol});
//         count++;
//     }
//     if (snap != m_lastSnapshot) {
//         std::cout << snap.toString(sequenceNo, m_symbol) << "\n";
//         m_lastSnapshot = snap;
//     }
// }