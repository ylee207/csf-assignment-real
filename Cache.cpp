#include "Cache.h"
#include <iostream>

using std::cout;
using std::endl;

// Cache constructor, initialized with cache properties and options
Cache::Cache(int numSets, int numBlocks, int numBytesPerBlock,
             const std::string& writeAllocateOption, 
             const std::string& writeThroughOption, 
             const std::string& replacementPolicy)
    : numSets(numSets), 
      numBlocks(numBlocks),
      numBytesPerBlock(numBytesPerBlock),
      writeAllocateOption(writeAllocateOption),
      writeThroughOption(writeThroughOption),
      lruOrFifo(replacementPolicy),
      totalLoads(0),
      totalStores(0),
      loadTotalHits(0),
      loadTotalMisses(0),
      storeTotalHits(0),
      storeTotalMisses(0),
      totalCycles(0)
{    
    // Set write-allocate policy based on input string
    if (writeAllocateOption == "write-allocate") {
        isWriteAllocate = true;
    } else {
        isWriteAllocate = false;
    }

    // Set write-through policy based on input string
    if (writeThroughOption == "write-through") {
        isWriteThrough = true;
    } else {
        isWriteThrough = false;
    }

    // Resize the sets vector to the given number of sets, initialized with the nullptr
    sets.resize(numSets, nullptr);
}

// Cache destructor to free dynamically allocated memory
Cache::~Cache() {
    for (Set* set : sets) {
        delete set;
    }
}

// Method to handle 'load' operations in cache
void Cache::load(unsigned int tag, unsigned int index) {
    totalLoads++; // Increment total load operations counter
    bool load = true;

    // If set at given index doesn't exist, create it
    if (sets[index] == nullptr) {
        sets[index] = new Set(isWriteAllocate, isWriteThrough, 
        lruOrFifo, numBlocks, numBytesPerBlock); 
    }
    
    // Access the set at given index
    Set* set = sets[index];
    cacheInfo * foundSlot = set->findSlotByTag(tag, load, isWriteThrough, isWriteAllocate, &totalCycles);
    
    // Increment the appropriate counter based on if it's a hit or miss
    if (foundSlot->hit == true) { // hit
        loadTotalHits++;
    }
    else { // miss
        loadTotalMisses++;
    }
}

// Method to handle 'store' operations in cache
void Cache::store(unsigned int tag, unsigned int index) {
    totalStores++; // Increment total store operations counter
    bool load = false;

    // If set at given index doesn't exist, create it
    if (sets[index] == nullptr) {
        sets[index] = new Set(isWriteAllocate, isWriteThrough, 
        lruOrFifo, numBlocks, numBytesPerBlock); 
    }

    // Access the set at given index
    Set* set = sets[index];
    cacheInfo * foundSlot;
    foundSlot = set->findSlotByTag(tag, load, isWriteThrough, isWriteAllocate, &totalCycles);

    // Increment the appropriate counter based on if it's a hit or miss
    if (foundSlot->hit) { // hit
        storeTotalHits++;
    } else { // miss
        storeTotalMisses++;
    }

    // Free dynamically allocated memory for the cacheInfo object
    delete foundSlot;
}

// Method to print cache statistics
void Cache::printStatistics() const {
    cout << "Total loads: " << totalLoads << endl;
    cout << "Total stores: " << totalStores << endl;
    cout << "Load hits: " << loadTotalHits << endl;
    cout << "Load misses: " << loadTotalMisses << endl;
    cout << "Store hits: " << storeTotalHits << endl;
    cout << "Store misses: " << storeTotalMisses << endl;
    cout << "Total cycles: " << totalCycles << endl;
}