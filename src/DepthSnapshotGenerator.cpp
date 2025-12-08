//
// Created by marco on 05/12/2025.
//

#include "../include/DepthSnapshotGenerator.h"

#include <iostream>

DepthSnapshotGenerator::DepthSnapshotGenerator(int depthLevels, OrderFileReader orderFileReader)
: m_orderFileReader(std::move(orderFileReader)),
  m_depthSnapshot(depthLevels)
{}

void DepthSnapshotGenerator::start()
{
    m_orderFileReader.start();

    try
    {
        std::cout << "Starting to process order messages" << std::endl;

        InputOrderMessage newOrder;
        while (m_orderFileReader.next(newOrder))
        {
            m_depthSnapshot.addNewOrder(newOrder);
        }
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Error processing order messages, error was: " << ex.what() << "\n";
    }
}
