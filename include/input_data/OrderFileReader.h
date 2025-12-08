//
// Created by marco on 08/12/2025.
//

#ifndef ORDERFILEREADER_H
#define ORDERFILEREADER_H
#include <fstream>
#include <cstdint>

#include "../InputOrderMessage.h"

struct InputOrderMessage;

class OrderFileReader {
public:
    explicit OrderFileReader(const std::string & filename);

    void start() const;

    bool next(InputOrderMessage& retOrder);

private:
    std::ifstream m_inputFile;
    uint32_t m_sequenceNo;
};

#endif //ORDERFILEREADER_H
