//
// Created by marco on 08/12/2025.
//

#include <cassert>
#include <iostream>
#include <sstream>

#include "model/OrderBook.h"

// Test Add orders
void testAddOrder() {
    OrderBook book("VC0", 3);

    std::string expected = "1, VC0, [(318800, 4709)], []\n2, VC0, [(318800, 4709), (315000, 2986)], []\n";
    InputOrderMessage addBid1{OrderType::ADD, "VC0", 1, OrderSide::BUY, 318800, 4709, 0};
    InputOrderMessage addBid2{OrderType::ADD, "VC0", 2, OrderSide::BUY, 315000, 2986, 0};

    // Start capturing cout
    std::stringstream buffer;
    std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());

    book.receiveNewOrder(addBid1, 1);
    book.receiveNewOrder(addBid2, 2);

    std::cout.rdbuf(oldCout);
    std::string output = buffer.str();

    assert(output == expected);
    std::cout << "✅ testAddOrder passed\n";
}

// Test Update order
void testUpdateOrder() {
    OrderBook book("VC0", 3);

    std::string expected = "1, VC0, [(318800, 4709)], []\n2, VC0, [(318800, 3000)], []\n";
    InputOrderMessage addBid{OrderType::ADD, "VC0", 1, OrderSide::BUY, 318800, 4709, 0};
    InputOrderMessage updateBid{OrderType::UPDATE, "VC0", 1, OrderSide::BUY, 318800, 3000, 0};

    // Start capturing cout
    std::stringstream buffer;
    std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());

    book.receiveNewOrder(addBid, 1);
    book.receiveNewOrder(updateBid, 2);

    std::cout.rdbuf(oldCout);
    std::string output = buffer.str();

    assert(output == expected);
    std::cout << "✅ testUpdateOrder passed\n";
}

// Test Execute order
void testExecuteOrder() {
    OrderBook book("VC0", 3);

    std::string expected = "1, VC0, [], [(320000, 100)]\n2, VC0, [], []\n"; // Fully executed, no orders left
    InputOrderMessage addAsk{OrderType::ADD, "VC0", 2, OrderSide::SELL, 320000, 100, 0};
    InputOrderMessage execAsk{OrderType::EXECUTE, "VC0", 2, OrderSide::SELL, 0, 0, 100};

    // Start capturing cout
    std::stringstream buffer;
    std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());

    book.receiveNewOrder(addAsk, 1);
    book.receiveNewOrder(execAsk, 2);

    std::cout.rdbuf(oldCout);
    std::string output = buffer.str();

    assert(output == expected);
    std::cout << "✅ testExecuteOrder passed\n";
}

void testAddOrder_NoSnapshotNeeded() {
    OrderBook book("VC0", 3);

    std::string expected = "10, VC0, [], [(350000, 100)]\n11, VC0, [], [(350000, 100), (360000, 100)]\n12, VC0, [], [(350000, 100), (360000, 100), (370000, 100)]\n";
    InputOrderMessage addAsk1{OrderType::ADD, "VC0", 1, OrderSide::SELL, 350000, 100, 0};
    InputOrderMessage addAsk2{OrderType::ADD, "VC0", 2, OrderSide::SELL, 360000, 100, 0};
    InputOrderMessage addAsk3{OrderType::ADD, "VC0", 3, OrderSide::SELL, 370000, 100, 0};
    InputOrderMessage addAsk4{OrderType::ADD, "VC0", 4, OrderSide::SELL, 380000, 100, 0};
    InputOrderMessage addAsk5{OrderType::ADD, "VC0", 5, OrderSide::SELL, 390000, 100, 0};

    // Start capturing cout
    std::stringstream buffer;
    std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());

    book.receiveNewOrder(addAsk1, 10);
    book.receiveNewOrder(addAsk2, 11);
    book.receiveNewOrder(addAsk3, 12);
    book.receiveNewOrder(addAsk4, 13);
    book.receiveNewOrder(addAsk5, 14);

    std::cout.rdbuf(oldCout);
    std::string output = buffer.str();

    assert(output == expected);
    std::cout << "✅ testAddOrder_NoSnapshotNeeded passed\n";
}

void testCancelOrder_NoSnapshotNeeded()
{
    OrderBook book("VC0", 3);

    std::string expected = "10, VC0, [], [(350000, 100)]\n11, VC0, [], [(350000, 100), (360000, 100)]\n12, VC0, [], [(350000, 100), (360000, 100), (370000, 100)]\n";
    InputOrderMessage addAsk1{OrderType::ADD, "VC0", 1, OrderSide::SELL, 350000, 100, 0};
    InputOrderMessage addAsk2{OrderType::ADD, "VC0", 2, OrderSide::SELL, 360000, 100, 0};
    InputOrderMessage addAsk3{OrderType::ADD, "VC0", 3, OrderSide::SELL, 370000, 100, 0};
    InputOrderMessage addAsk4{OrderType::ADD, "VC0", 4, OrderSide::SELL, 380000, 100, 0};
    InputOrderMessage addUpdate4{OrderType::UPDATE, "VC0", 4, OrderSide::SELL, 380000, 10, 0};
    InputOrderMessage addDelete4{OrderType::DELETE, "VC0", 4, OrderSide::SELL, 380000, 10, 0};
    InputOrderMessage addAsk5{OrderType::ADD, "VC0", 5, OrderSide::SELL, 390000, 100, 0};
    InputOrderMessage addExecute5{OrderType::EXECUTE, "VC0", 5, OrderSide::SELL, 390000, 40, 0};

    // Start capturing cout
    std::stringstream buffer;
    std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());

    book.receiveNewOrder(addAsk1, 10);
    book.receiveNewOrder(addAsk2, 11);
    book.receiveNewOrder(addAsk3, 12);
    book.receiveNewOrder(addAsk4, 13);
    book.receiveNewOrder(addUpdate4, 14);
    book.receiveNewOrder(addDelete4, 15);
    book.receiveNewOrder(addAsk5, 16);
    book.receiveNewOrder(addExecute5, 17);

    std::cout.rdbuf(oldCout);
    std::string output = buffer.str();

    assert(output == expected);
    std::cout << "✅ testCancelOrder_NoSnapshotNeeded passed\n";
}

void testExample1()
{
    OrderBook book("VC0", 3);

    std::string expected = "3, VC0, [], []\n";
    InputOrderMessage addAsk1{OrderType::ADD, "VC0", 1, OrderSide::SELL, 390000, 100, 0};
    InputOrderMessage addAsk2{OrderType::ADD, "VC0", 2, OrderSide::SELL, 380000, 100, 0};
    InputOrderMessage addAsk3{OrderType::ADD, "VC0", 3, OrderSide::SELL, 370000, 100, 0};
    InputOrderMessage addAsk4{OrderType::ADD, "VC0", 4, OrderSide::SELL, 360000, 100, 0};
    InputOrderMessage addAsk5{OrderType::ADD, "VC0", 5, OrderSide::SELL, 350000, 100, 0};

    // Start capturing cout
    std::stringstream buffer;
    std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());

    book.receiveNewOrder(addAsk1, 10);
    book.receiveNewOrder(addAsk2, 11);
    book.receiveNewOrder(addAsk3, 12);
    book.receiveNewOrder(addAsk4, 13);
    book.receiveNewOrder(addAsk5, 14);

    std::cout.rdbuf(oldCout);
    std::string output = buffer.str();

    assert(output == expected);
    std::cout << "✅ testExample1 passed\n";
}

int main() {
    testAddOrder();
    testUpdateOrder();
    testExecuteOrder();
    testAddOrder_NoSnapshotNeeded();
    testCancelOrder_NoSnapshotNeeded();
    // testExample1();

    std::cout << "✅ All 'no snapshot needed' tests passed!" << std::endl;
    return 0;
}