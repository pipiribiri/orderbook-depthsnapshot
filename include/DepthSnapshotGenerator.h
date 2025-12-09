//
// Created by marco on 05/12/2025.
//

#ifndef DEPTHSNAPSHOTGENERATOR_H
#define DEPTHSNAPSHOTGENERATOR_H

#include <memory>
#include <unordered_map>

#include "input_data/OrderFileReader.h"
#include "model/OrderBook.h"


class DepthSnapshotGenerator {
public:
    DepthSnapshotGenerator(int depthLevels, OrderFileReader orderFileReader);

    void start();

private:
    OrderFileReader m_orderFileReader;
    int             m_depthLevels;
    std::unordered_map<std::string, std::unique_ptr<OrderBook>> m_orderBooks;
};

#endif //DEPTHSNAPSHOTGENERATOR_H
