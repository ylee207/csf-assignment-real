#ifndef CACHE_H
#define CACHE_H

#include <vector>
#include <map>
#include <string>
#include <cstdint>
#include "Set.h"

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
    Cache(int numSets, int numBlocks, int numBytesPerBlock, 
          const std::string& writeAllocateOption, 
          const std::string& writeThroughOption, 
          const std::string& replacementPolicy); 

    ~Cache();
    void load(unsigned int tag, unsigned int index);

    void store(unsigned int tag, unsigned int index);

    void printStatistics() const;
};

#endif
