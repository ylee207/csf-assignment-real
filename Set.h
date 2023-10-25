#ifndef SET_H
#define SET_H

#include <cstdint>
#include <string>
#include <vector>
#include "Slot.h"
#include "cacheInfo.h"

class Set {
public:
    Set(bool isWriteAllocate, bool isWriteThrough, std::string& lruOrFifo, unsigned numBlocks, unsigned numBytesPerBlock);  // Constructor
    ~Set(); // Destructor

    cacheInfo * findSlotByTag(uint32_t tag, bool load);
    Slot* findActualSlotByTag(uint32_t tag);
    cacheInfo * addNewSlot(uint32_t tag);
    void evictSlot(uint32_t tag);  // LRU or FIFO depending on what you choose
    std::vector<Slot> slots;

private:
    bool writeAllocate; 
    bool writeThrough; 
    std::string& evictStrat; 
    unsigned maxBlocks;
    unsigned numBytes;
    unsigned currtimeStamp;
};

#endif
