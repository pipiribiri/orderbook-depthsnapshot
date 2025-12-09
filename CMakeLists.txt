cmake_minimum_required(VERSION 3.31)
project(OrderBookFeed)

set(CMAKE_CXX_STANDARD 17)

# Core library with reusable sources
add_library(orderbook_lib
        src/DepthSnapshotGenerator.cpp
        src/model/OrderBook.cpp
        src/input_data/InputOrderParser.cpp
        src/input_data/OrderFileReader.cpp
        src/model/DepthSnapshot.cpp
)

# Add absolute include path (MinGW-safe)
target_include_directories(orderbook_lib PUBLIC ${CMAKE_SOURCE_DIR}/include)

# Main executable
add_executable(OrderBookFeed src/main.cpp)
target_link_libraries(OrderBookFeed PRIVATE orderbook_lib)
target_include_directories(OrderBookFeed PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Unit tests
enable_testing()
add_executable(unit_tests tests/TestDepthSnapshot.cpp)
target_link_libraries(unit_tests PRIVATE orderbook_lib)
target_include_directories(unit_tests PRIVATE ${CMAKE_SOURCE_DIR}/include)
add_test(NAME unit_tests COMMAND unit_tests)