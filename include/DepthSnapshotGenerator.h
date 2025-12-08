//
// Created by marco on 05/12/2025.
//

#ifndef DEPTHSNAPSHOTGENERATOR_H
#define DEPTHSNAPSHOTGENERATOR_H
#include <cstdint>

#include "model/DepthSnapshot.h"
#include "InputOrderMessage.h"
#include "input_data/OrderFileReader.h"


class DepthSnapshotGenerator {
public:
    DepthSnapshotGenerator(int depthLevels, OrderFileReader orderFileReader);

    void start();

private:
    OrderFileReader m_orderFileReader;
    DepthSnapshot   m_depthSnapshot;
};

#endif //DEPTHSNAPSHOTGENERATOR_H
