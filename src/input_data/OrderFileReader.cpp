//
// Created by marco on 08/12/2025.
//

#include "../../include/input_data/OrderFileReader.h"
#include "../../include/input_data/InputOrderParser.h"
OrderFileReader::OrderFileReader(const std::string &filename)
: m_inputFile(filename, std::ios::binary)
{}

void OrderFileReader::start() const
{
    if (!m_inputFile)
    {
        throw std::runtime_error("Failed to open input file");
    }
}

bool OrderFileReader::next(InputOrderMessage &retOrder, uint32_t& retSequenceNumber)
{
    if (!m_inputFile.good()) return false;

    retSequenceNumber = InputOrderParser::readSequenceNo(m_inputFile);
    const uint32_t messageSize = InputOrderParser::readMessageSize(m_inputFile);
    (void)messageSize; // not used directly, but could validate

    retOrder.type    = InputOrderParser::readOrderType(m_inputFile);
    retOrder.symbol  = InputOrderParser::readSymbol(m_inputFile);
    retOrder.orderId = InputOrderParser::readOrderId(m_inputFile);
    retOrder.side    = InputOrderParser::readSide(m_inputFile);
    InputOrderParser::skipReserved(m_inputFile, 3);

    retOrder.volume = 0;
    retOrder.price = 0;
    retOrder.tradedQuantity = 0;

    if (retOrder.type == OrderType::EXECUTE)
    {
        retOrder.tradedQuantity = InputOrderParser::readTradedQuantity(m_inputFile);
    }
    else if (retOrder.type == OrderType::ADD || retOrder.type == OrderType::UPDATE)
    {
        retOrder.volume  = InputOrderParser::readVolume(m_inputFile);
        retOrder.price = InputOrderParser::readPrice(m_inputFile);
        InputOrderParser::skipReserved(m_inputFile, 4);
    }
   
    return true;
}
