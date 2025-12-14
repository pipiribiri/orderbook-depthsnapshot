# OrderBookDepthSnapshot
A high-performance C++17 market data order book engine designed to process Add/Update/Delete/Execute messages and produce efficient depth snapshots. The project focuses on speed, memory locality, and clean architecture, making it suitable for trading infrastructure, feed handlers, and exchange simulators.

## Features

### Core Functionality
- Maintains bid and ask books with correct price-time semantics.
- Supports all common order message types:
  - Add
  - Update
  - Delete
  - Execute (partial or full fills)
- Produces top-N depth snapshots for downstream consumers.

### Performance Optimizations
- Compile-time comparators (std::greater / std::less) for fast ordered maps.
- O(1) order lookup via unordered_map.
- Level counters to detect structural changes and avoid unnecessary snapshot rebuilds.
- Cached best bid/ask for instant access.
- Snapshot generation only when relevant changes occur.

### Clean Architecture
- OrderList manages one side of the book (bids or asks).
- OrderBook orchestrates message handling and snapshot publishing.
- DepthSnapshot provides a clean, serializable representation of the book state.

## Project Structure

OrderBookDepthSnapshot/
│
├── include/
│   ├── model/
│   │   ├── OrderBook.h
│   │   ├── OrderList.h
│   │   ├── DepthSnapshot.h
│   │   ├── Message.h
│   │   └── ...
│   └── ...
│
├── src/
│   ├── model/
│   │   ├── OrderBook.cpp
│   │   ├── OrderList.cpp (if needed)
│   │   └── ...
│   └── main.cpp
│
├── CMakeLists.txt
└── README.md

## Requirements
- C++17
- CMake ≥ 3.15
- GCC, Clang, or MSVC (CLion toolchain recommended)

## Building the Project

### Using CLion
1. Open the project folder in CLion.
2. CLion automatically configures CMake.
3. Press Build or Run.

### Using terminal
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j

## How It Works

### OrderList
Each side of the book (bids/asks) maintains:
- A sorted std::map<price → volume>
- An unordered_map<orderId → {price, size}>
- A cached bestPrice_
- A levelCount_ to detect structural changes

This gives:
- Fast updates
- Fast best-price access
- Efficient snapshot generation

### OrderBook
- Routes messages to the correct side.
- Determines whether a snapshot must be rebuilt.
- Publishes snapshots only when meaningful changes occur.

### DepthSnapshot
- Holds top-N bid/ask levels.
- Provides a clean toString() for logging or feed output.

## Example Output

10234, AAPL, [(15000, 200), (14999, 100)], [(15001, 50), (15002, 80)]

Format:
sequence, symbol, [bid levels], [ask levels]

## Testing
You can feed synthetic or real market data messages into OrderBook::handleMessage() and verify:
- Correct depth reconstruction
- Correct best bid/ask tracking
- Snapshot changes only when necessary

## Future Improvements
- SIMD-accelerated snapshot building (via intrinsics or xsimd)
- Replace std::map with flat_map for better cache locality
- Add unit tests for edge cases (crossed book, empty book, large updates)
- Add support for multiple symbols via unordered_map<string, OrderBook>
