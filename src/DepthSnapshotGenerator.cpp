//
// Created by marco on 05/12/2025.
//

#include "../include/DepthSnapshotGenerator.h"

#include <iostream>

DepthSnapshotGenerator::DepthSnapshotGenerator(int m_depthLevels, OrderFileReader orderFileReader)
: m_orderFileReader(std::move(orderFileReader)),
  m_depthLevels(m_depthLevels)
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
            if (m_orderBooks.find(newOrder.symbol) == std::end(m_orderBooks))
            {
                std::cout << "Creating snapshot data for symbol: " << newOrder.symbol << std::endl;
                m_orderBooks.emplace(newOrder.symbol, std::make_unique<OrderBook>(OrderBook(newOrder.symbol, m_depthLevels)));
            }
            m_orderBooks[newOrder.symbol]->receiveNewOrder(newOrder, sequenceNumber);
        }
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Error processing order messages, error was: " << ex.what() << "\n";
    }
}
