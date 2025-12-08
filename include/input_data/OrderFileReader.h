//
// Created by marco on 08/12/2025.
//

#ifndef ORDERFILEREADER_H
#define ORDERFILEREADER_H
#include <fstream>
#include <cstdint>

#include "../model/InputOrderMessage.h"

struct InputOrderMessage;

class OrderFileReader {
public:
    explicit OrderFileReader(const std::string & filename);

    void start() const;

    bool next(InputOrderMessage &retOrder, uint32_t& retSequenceNumber);

private:
    std::ifstream m_inputFile;
};

#endif //ORDERFILEREADER_H
