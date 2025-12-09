//
// Created by marco on 07/12/2025.
//

#include "input_data/InputOrderParser.h"

#include <istream>

uint32_t InputOrderParser::readSequenceNo(std::istream& in)
{
    uint32_t seq;
    in.read(reinterpret_cast<char*>(&seq), 4);
    return seq;
}

uint32_t InputOrderParser::readMessageSize(std::istream& in)
{
    uint32_t size;
    in.read(reinterpret_cast<char*>(&size), 4);
    return size;
}

OrderType InputOrderParser::readOrderType(std::istream& in)
{
    char typeChar;
    in.read(&typeChar, 1);
    switch (typeChar)
    {
        case 'A': return OrderType::ADD;
        case 'U': return OrderType::UPDATE;
        case 'D': return OrderType::DELETE;
        case 'E': return OrderType::EXECUTE;
        default:  throw std::runtime_error("Unknown order type");
    }
}

std::string InputOrderParser::readSymbol(std::istream& in)
{
    char symbol[3];
    in.read(symbol, 3);
    return std::string(symbol, 3);
}

uint64_t InputOrderParser::readOrderId(std::istream& in)
{
    uint64_t id;
    in.read(reinterpret_cast<char*>(&id), 8);
    return id;
}

OrderSide InputOrderParser::readSide(std::istream &in)
{
    char side;
    in.read(&side, 1);

    switch (side)
    {
        case 'B': return OrderSide::BUY;
        case 'S': return OrderSide::SELL;
        default:  throw std::runtime_error("Unknown order side");
    }
}

void InputOrderParser::skipReserved(std::istream& in, const int count)
{
    in.ignore(count);
}

uint64_t InputOrderParser::readVolume(std::istream& in)
{
    uint64_t size;
    in.read(reinterpret_cast<char*>(&size), 8);
    return size;
}

int32_t InputOrderParser::readPrice(std::istream& in)
{
    int32_t price;
    in.read(reinterpret_cast<char*>(&price), 4);
    return price;
}

uint64_t InputOrderParser::readTradedQuantity(std::istream& in)
{
    uint64_t qty;
    in.read(reinterpret_cast<char*>(&qty), 8);
    return qty;
}