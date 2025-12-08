//
// Created by marco on 05/12/2025.
//

#ifndef DEPTHSNAPSHOTGENERATOR_H
#define DEPTHSNAPSHOTGENERATOR_H

#include <memory>
#include <unordered_map>

#include "input_data/OrderFileReader.h"
#include "model/DepthSnapshot.h"


class DepthSnapshotGenerator {
public:
    DepthSnapshotGenerator(int depthLevels, OrderFileReader orderFileReader);

    void start();

private:
    OrderFileReader m_orderFileReader;
    int             m_depthLevel;
    std::unordered_map<std::string, std::unique_ptr<DepthSnapshot>> m_snapshots;
};

#endif //DEPTHSNAPSHOTGENERATOR_H
