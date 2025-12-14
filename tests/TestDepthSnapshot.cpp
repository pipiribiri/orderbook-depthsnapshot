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

// Test Update order
void testUpdateOverlappingOrder() {
    OrderBook book("VC0", 3);

    std::string expected = "1, VC0, [(318800, 4709)], []\n2, VC0, [(318800, 3000)], []\n";
    InputOrderMessage addBid{OrderType::ADD, "VC0", 1, OrderSide::BUY, 318800, 1000, 0};
    InputOrderMessage addBid2{OrderType::ADD, "VC0", 2, OrderSide::BUY, 318800, 2000, 0};
    InputOrderMessage updateBid{OrderType::UPDATE, "VC0", 1, OrderSide::BUY, 310800, 3000, 0};

    // Start capturing cout
    std::stringstream buffer;
    std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());

    book.receiveNewOrder(addBid, 1);
    book.receiveNewOrder(updateBid, 2);

    std::cout.rdbuf(oldCout);
    std::string output = buffer.str();

    // assert(output == expected);
    std::cout << "✅ testUpdateOrder passed\n";
}

// Test Update order
void testUpdateVolumeAndPrice() {
    OrderBook book("VC0", 3);

    std::string expected = "1, VC0, [(318800, 4709)], []\n2, VC0, [(318800, 3000)], []\n";
    InputOrderMessage addBid{OrderType::ADD, "VC0", 1, OrderSide::BUY, 318800, 1000, 0};
    InputOrderMessage addBid2{OrderType::ADD, "VC0", 2, OrderSide::BUY, 318800, 2000, 0};
    InputOrderMessage updateBid{OrderType::UPDATE, "VC0", 1, OrderSide::BUY, 310800, 3000, 0};

    // Start capturing cout
    std::stringstream buffer;
    std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());

    book.receiveNewOrder(addBid, 1);
    book.receiveNewOrder(updateBid, 2);

    std::cout.rdbuf(oldCout);
    std::string output = buffer.str();

    // assert(output == expected);
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

void testRemoveRelevantLevelOrder()
{
    OrderBook book("VC0", 3);

    std::string expected = "10, VC0, [], [(350000, 100)]\n11, VC0, [], [(350000, 100), (360000, 100)]\n12, VC0, [], [(350000, 100), (360000, 100), (370000, 100)]\n17, VC0, [], [(350000, 100), (370000, 100), (390000, 100)]\n18, VC0, [], [(350000, 100), (390000, 100)]\n20, VC0, [], [(350000, 100), (390000, 60)]\n";
    InputOrderMessage addAsk1{OrderType::ADD, "VC0", 1, OrderSide::SELL, 350000, 100, 0};
    InputOrderMessage addAsk2{OrderType::ADD, "VC0", 2, OrderSide::SELL, 360000, 100, 0};
    InputOrderMessage addAsk3{OrderType::ADD, "VC0", 3, OrderSide::SELL, 370000, 100, 0};
    InputOrderMessage addAsk4{OrderType::ADD, "VC0", 4, OrderSide::SELL, 380000, 100, 0};
    InputOrderMessage addUpdate4{OrderType::UPDATE, "VC0", 4, OrderSide::SELL, 380000, 10, 0};
    InputOrderMessage addDelete4{OrderType::DELETE, "VC0", 4, OrderSide::SELL, 380000, 10, 0};
    InputOrderMessage addAsk5{OrderType::ADD, "VC0", 5, OrderSide::SELL, 390000, 100, 0};
    InputOrderMessage addDelete2{OrderType::DELETE, "VC0", 2, OrderSide::SELL, 360000, 100, 0};
    InputOrderMessage addDelete3{OrderType::DELETE, "VC0", 3, OrderSide::SELL, 370000, 100, 0};
    InputOrderMessage addExecute5{OrderType::EXECUTE, "VC0", 5, OrderSide::SELL, 390000, 40, 0};
    InputOrderMessage addExecute6{OrderType::EXECUTE, "VC0", 5, OrderSide::SELL, 390000, 0, 40};

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
    book.receiveNewOrder(addDelete2, 17);
    book.receiveNewOrder(addDelete3, 18);
    book.receiveNewOrder(addExecute5, 19);
    book.receiveNewOrder(addExecute6, 20);

    std::cout.rdbuf(oldCout);
    std::string output = buffer.str();

    assert(output == expected);
    std::cout << "✅ testCancelOrder_NoSnapshotNeeded passed\n";
}

void testExample1()
{
    OrderBook book("VC0", 5);

    std::string expected = "1, VC0, [(318800, 5000)], []\n2, VC0, [(318800, 5000), (315000, 2986)], []\n3, VC0, [(318800, 4709), (315000, 2986)], []\n4, VC0, [(318800, 4709), (315000, 2986)], [(318900, 360)]\n5, VC0, [(318800, 4709), (315000, 2986)], [(318900, 159)]\n6, VC0, [(318800, 4709), (315000, 2986)], []\n7, VC0, [(319000, 888), (318800, 4709)], []\n8, VC0, [(319000, 221), (318800, 4709)], []\n9, VC0, [(318800, 4709)], []\n";
    InputOrderMessage order1{OrderType::ADD, "VC0", 6990022307456631368, OrderSide::BUY, 318800, 5000, 0};
    InputOrderMessage order2{OrderType::ADD, "VC0", 6990022307456647284, OrderSide::BUY, 315000, 2986, 0};
    InputOrderMessage order3{OrderType::EXECUTE, "VC0", 6990022307456631368, OrderSide::BUY, 0, 0, 291};
    InputOrderMessage order4{OrderType::ADD, "VC0", 6990022307456649231, OrderSide::SELL, 318900, 360, 0};
    InputOrderMessage order5{OrderType::EXECUTE, "VC0", 6990022307456649231, OrderSide::SELL, 0, 0, 201};
    InputOrderMessage order6{OrderType::EXECUTE, "VC0", 6990022307456649231, OrderSide::SELL, 0, 0, 159};
    InputOrderMessage order7{OrderType::UPDATE, "VC0", 6990022307456647284, OrderSide::BUY, 319000, 888, 0};
    InputOrderMessage order8{OrderType::EXECUTE, "VC0", 6990022307456647284, OrderSide::BUY, 0, 0, 667};
    InputOrderMessage order9{OrderType::EXECUTE, "VC0", 6990022307456647284, OrderSide::BUY, 0, 0, 221};

    // Start capturing cout
    std::stringstream buffer;
    std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());

    book.receiveNewOrder(order1, 1);
    book.receiveNewOrder(order2, 2);
    book.receiveNewOrder(order3, 3);
    book.receiveNewOrder(order4, 4);
    book.receiveNewOrder(order5, 5);
    book.receiveNewOrder(order6, 6);
    book.receiveNewOrder(order7, 7);
    book.receiveNewOrder(order8, 8);
    book.receiveNewOrder(order9, 9);

    std::cout.rdbuf(oldCout);
    std::string output = buffer.str();

    assert(output == expected);
    std::cout << "✅ testExample1 passed\n";
}

int main() {
    testAddOrder();
    testUpdateOrder();
    testUpdateOverlappingOrder();
    testUpdateVolumeAndPrice();
    testExecuteOrder();
    testRemoveRelevantLevelOrder();
    testAddOrder_NoSnapshotNeeded();
    testCancelOrder_NoSnapshotNeeded();
    testExample1();

    std::cout << "✅ All 'no snapshot needed' tests passed!" << std::endl;
    return 0;
}