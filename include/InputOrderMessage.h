//
// Created by marco on 05/12/2025.
//

#ifndef ORDERMESSAGE_H
#define ORDERMESSAGE_H
#include <cstdint>
#include <string>

enum class OrderType { ADD, UPDATE, DELETE, EXECUTE };

enum class OrderSide { BUY, SELL };

inline std::ostream& operator<<(std::ostream& os, const OrderType type)
{
    switch (type)
    {
        case OrderType::ADD:     os << std::string("ADD"); break;
        case OrderType::UPDATE:  os << std::string("UPDATE"); break;
        case OrderType::DELETE:  os << std::string("DELETE"); break;
        case OrderType::EXECUTE: os << std::string("EXECUTE"); break;
    }
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const OrderSide type)
{
    switch (type)
    {
        case OrderSide::BUY:     os << std::string("BUY"); break;
        case OrderSide::SELL:  os << std::string("SELL"); break;
    }
    return os;
}

struct MessageHeader {
    std::uint32_t sequenceNo;
    uint32_t messageSize;
};

struct InputOrderMessage {
    OrderType type;
    std::string symbol;
    uint64_t orderId;
    OrderSide side;
    int64_t price;
    uint64_t volume;
    uint64_t tradedQuantity;
};

#endif //ORDERMESSAGE_H
