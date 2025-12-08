//
// Created by marco on 05/12/2025.
//

#include "../include/DepthSnapshotGenerator.h"

#include <iostream>

DepthSnapshotGenerator::DepthSnapshotGenerator(int depthLevels, OrderFileReader orderFileReader)
: m_orderFileReader(std::move(orderFileReader)),
  m_depthLevel(depthLevels)
{}

void DepthSnapshotGenerator::start()
{
    m_orderFileReader.start();

    try
    {
        std::cout << "Starting to process order messages" << std::endl;

        InputOrderMessage newOrder;
        uint32_t sequenceNumber;
        while (m_orderFileReader.next(newOrder, sequenceNumber))
        {
            if (m_snapshots.find(newOrder.symbol) == std::end(m_snapshots))
            {
                std::cout << "Creating snapshot data for symbol: " << newOrder.symbol << std::endl;
                m_snapshots.emplace(newOrder.symbol, std::make_unique<DepthSnapshot>(DepthSnapshot(newOrder.symbol, m_depthLevel)));
            }
            m_snapshots[newOrder.symbol]->addNewOrder(newOrder, sequenceNumber);
        }
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Error processing order messages, error was: " << ex.what() << "\n";
    }
}
