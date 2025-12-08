//
// Created by marco on 05/12/2025.
//

#ifndef DEPTHSNAPSHOT_H
#define DEPTHSNAPSHOT_H
#include <cstdint>

#include "../InputOrderMessage.h"
#include "OrderList.h"

struct DepthLevel {
    std::int64_t price;
    uint64_t volume;
};

class DepthSnapshot {
public:
    explicit DepthSnapshot(int depthLevels);

    void publishSnapshot();

    void addNewOrder(const InputOrderMessage & newOrder);

    // std::vector<DepthLevel> bids;
    // std::vector<DepthLevel> asks;
    //
    // bool operator==(const DepthSnapshot &other) const;
    //
    // bool operator!=(const DepthSnapshot &other) const
    // {
    //     return !(*this == other);
    // }
    //
    // std::string toString(uint32_t sequenceNo, const std::string &symbol) const;



    // DepthSnapshot() = default;
    //
    //
    // int m_depthLevels;
    // std::unordered_map<uint64_t, Order> m_orders;
    //
    // std::map<int64_t, uint64_t> m_bids; // price -> volume
    // std::map<int64_t, uint64_t> m_asks; // price -> volume
    //
    // DepthSnapshot m_lastSnapshot;
    //
    //
    // void addOrder(const InputOrderMessage &msg);
    //
    // void updateOrder(const InputOrderMessage &msg);
    //
    // void deleteOrder(const InputOrderMessage &msg);
    //
    // void executeOrder(const InputOrderMessage &msg);
    //
    // void publishSnapshot(uint32_t sequenceNo, int depthLevels);
    //
    // std::map<int64_t, uint64_t> selectOrderBookSide(const OrderSide &orderSide);

private:
    int m_depthLevels;

    OrderList m_bids;
    OrderList m_asks;
};

#endif //DEPTHSNAPSHOT_H
