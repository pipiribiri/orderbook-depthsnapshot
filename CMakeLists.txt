cmake_minimum_required(VERSION 3.31)
project(OrderBookFeed)

set(CMAKE_CXX_STANDARD 17)

add_executable(OrderBookFeed src/main.cpp
        src/DepthSnapshotGenerator.cpp
        include/DepthSnapshotGenerator.h
        include/model/InputOrderMessage.h
        include/model/OrderBook.h
        src/model/OrderBook.cpp
        include/model/OrderList.h
        src/input_data/InputOrderParser.cpp
        include/input_data/InputOrderParser.h
        tests/TestDepthSnapshot.cpp
        src/input_data/OrderFileReader.cpp
        include/input_data/OrderFileReader.h
        src/model/DepthSnapshot.cpp
        include/model/DepthSnapshot.h
)
