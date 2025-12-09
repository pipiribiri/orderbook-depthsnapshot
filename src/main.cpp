#include <cstdint>
#include <iostream>

#include "DepthSnapshotGenerator.h"
#include "input_data/OrderFileReader.h"

int main(int argc, char* argv[])
{
     if (argc < 3) {
          std::cerr << "Usage: ./orderbook <depthLevels> <inputFile>\n";
          return 1;
     }

     const int depthLevels = std::stoi(argv[1]);
     const std::string inputFilename = argv[2];

     // std::cout << "Starting Depth Snapshot generator for input file " << inputFilename << " with " << depthLevels << " levels of depth." << std::endl;

     DepthSnapshotGenerator depthSnapshotGenerator(
          depthLevels,
          OrderFileReader(inputFilename));

     depthSnapshotGenerator.start();

     // std::cout << "Depth Snapshot generator finished." << std::endl;

     return 0;
}
