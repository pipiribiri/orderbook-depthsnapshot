//
// Created by marco on 08/12/2025.
//

#include "../../include/model/DepthSnapshot.h"

#include <iostream>
#include <sstream>

void DepthSnapshot::publishSnapshot(
        const uint32_t sequenceNumber,
        const std::string& symbol,
        const std::vector<DepthLevel> &bids,
        const std::vector<DepthLevel> &asks)
{
    std::ostringstream oss;
    oss << sequenceNumber << ", " << symbol << ", [";
    for (size_t i = 0; i < bids.size(); ++i) {
        if (i > 0) oss << ", ";
        oss << "(" << bids[i].price << ", " << bids[i].volume << ")";
    }
    oss << "], [";
    for (size_t i = 0; i < asks.size(); ++i) {
        if (i > 0) oss << ", ";
        oss << "(" << asks[i].price << ", " << asks[i].volume << ")";
    }
    oss << "]";
    std::cout << oss.str() << std::endl;
}
