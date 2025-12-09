//
// Created by marco on 08/12/2025.
//

#ifndef DEPTHSNAPSHOT_H
#define DEPTHSNAPSHOT_H
#include <vector>

#include "InputOrderMessage.h"

struct DepthLevel {
    int64_t price;
    uint64_t volume;
};

class DepthSnapshot {
public:
    static void publishSnapshot(
        uint32_t sequenceNumber,
        const std::string& symbol,
        const std::vector<DepthLevel> &bids,
        const std::vector<DepthLevel> &asks);
};

#endif //DEPTHSNAPSHOT_H
