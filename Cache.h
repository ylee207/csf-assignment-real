#ifndef CACHE_H
#define CACHE_H

#include <vector>
#include <map>
#include <string>
#include <cstdint>
#include "Set.h"

// Cache class
class Cache {
private:
    std::vector<Set*> sets;
    std::map<uint32_t, Slot*> index;
    int numSets;
    int numBlocks;
    int numBytesPerBlock;
    std::string writeAllocateOption;
    std::string writeThroughOption;
    bool isWriteAllocate;
    bool isWriteThrough;
    std::string lruOrFifo;
    unsigned totalLoads;
    unsigned totalStores;
    unsigned loadTotalHits;
    unsigned loadTotalMisses;
    unsigned storeTotalHits;
    unsigned storeTotalMisses;
    unsigned totalCycles;

public:
    // Cache constructor
    Cache(int numSets, int numBlocks, int numBytesPerBlock, 
          const std::string& writeAllocateOption, 
          const std::string& writeThroughOption, 
          const std::string& replacementPolicy); 

    // Destructor for Cache class
    ~Cache();

    // handles load operation
    void load(unsigned int tag, unsigned int index);

    // handles store operation
    void store(unsigned int tag, unsigned int index);

    // function to print out the statistics
    void printStatistics() const;
};

#endif
