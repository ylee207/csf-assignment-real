#include "Cache.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <sstream>
#include <cmath>
#include <bitset>

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::cin;

/*
 * Returns true of a number is not a power of two.
 */
bool isNotPowerOfTwo(unsigned int num);

/*
 * Function to display in binary format
 */
std::string toBinary(unsigned int value, int width);

/*
 * Main function for the program.
 */
int main(int argc, char* argv[]) {
    // handling command line arguments
    if (argc != 7) {
        cerr << "Incorrect number of arguments" << endl;
        return 1;
    }

    // get the number of sets from the command line argument.
    int numSets = std::atoi(argv[1]);
    // check if numSets is not a power of 2
    if (isNotPowerOfTwo(numSets)) {
        cerr << "argv[1] (numSets) is not a number that is a power of 2." << endl;
        return 1;
    }

    // get the number of blocks from the command line argument.
    int numBlocks = std::atoi(argv[2]);
    // check if numBlocks is not a power of 2
    if (isNotPowerOfTwo(numBlocks)) {
        cerr << "argv[2] (numBlocks) is not a number that is a power of 2." << endl;
        return 1;
    }

    // get the number of bytes per block from the command line argument.
    int numBytesPerBlock = std::atoi(argv[3]);
    // check if block size is less than 4
    if (numBytesPerBlock < 4 || isNotPowerOfTwo(numBytesPerBlock)) {
        cerr << "argv[3] (numBytesPerBlock) is not at least 4 bytes or a power of 2." << endl;
        return 1;
    }

    // get the options for write-allocate and write-through
    std::string writeAllocateOption(argv[4]);
    std::string writeThroughOption(argv[5]);
    // check if the writeAllocateOption is a valid option
    if (!(writeAllocateOption == "write-allocate" || writeAllocateOption == "no-write-allocate")) {
        cerr << "For argv[4], your only valid options are 'write-allocate' or 'no-write-allocate'." << endl;
        return 1;
    }

    // check if the writeThroughOption is a valid option
    if (!(writeThroughOption == "write-through" || writeThroughOption == "write-back")) {
        cerr << "For argv[5], your only valid options are 'write-through' or 'write-back'." << endl;
        return 1;
    }
    
    // check if 'write-back' and 'no-write-allocate' were both specified
    if (writeThroughOption == "write-back" && writeAllocateOption == "no-write-allocate") {
        cerr << "'write-back' and 'no-write-allocate' cannot be specified at the same time." << endl;
        return 1;
    }

    // designation for lru or fifo
    std::string lruOrFifo(argv[6]);

    // Declaration for variables to store values from the memory trace file.
    string line;
    string operation;
    unsigned int address;
    int value;
    
    // Declaring a cache
    Cache cache(numSets, numBlocks, numBytesPerBlock, writeAllocateOption, writeThroughOption, lruOrFifo);

    // Read each line in .trace files and perform cache operations depending on load or store
    while (std::getline(cin, line)) {
        std::istringstream iss(line);
        if (iss >> operation >> std::hex >> address >> value) {
            unsigned tag = (address >> unsigned(log2(numBytesPerBlock)));
            unsigned index = tag % numSets; 

            if (operation == "l") {
                cache.load(tag, index);
            } else if (operation == "s") {
                cache.store(tag, index);
            }
        } else {
            std::cerr << "Failed to parse line: " << line << endl;
            return 1;
        }
    }

    // print the statistics for number of cycles, loads, stores, etc.
    cache.printStatistics();
    
    return 0;
}

bool isNotPowerOfTwo(unsigned int num) {
    return num == 0 || ((num & (num - 1)) != 0);
}

std::string toBinary(unsigned int value, int width) {
    return std::bitset<32>(value).to_string().substr(32-width, width);
}