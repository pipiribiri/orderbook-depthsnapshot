//
// Created by marco on 07/12/2025.
//

#ifndef INPUTORDERDESERIALIZER_H
#define INPUTORDERDESERIALIZER_H
#include <cstdint>
#include <iosfwd>

#include "model/InputOrderMessage.h"

class InputOrderParser {
public:
    // Reader functions for each field
    static std::uint32_t readSequenceNo(std::istream& in);
    static uint32_t readMessageSize(std::istream& in);
    static OrderType  readOrderType(std::istream& in);
    static std::string readSymbol(std::istream& in);
    static uint64_t readOrderId(std::istream& in);
    static OrderSide readSide(std::istream &in);
    static void     skipReserved(std::istream& in, int count);
    static uint64_t readVolume(std::istream& in);
    static int32_t  readPrice(std::istream& in);
    static uint64_t readTradedQuantity(std::istream& in);
};



#endif //INPUTORDERDESERIALIZER_H
