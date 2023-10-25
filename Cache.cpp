#include "Cache.h"
#include <iostream>

using std::cout;
using std::endl;

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
    if (writeAllocateOption == "write-allocate") {
        isWriteAllocate = true;
    } else {
        isWriteAllocate = false;
    }

    if (writeThroughOption == "write-through") {
        isWriteThrough = true;
    } else {
        isWriteThrough = false;
    }

    sets.resize(numSets, nullptr);
}

Cache::~Cache() {
    for (Set* set : sets) {
        delete set;
    }
}

void Cache::load(unsigned int address, unsigned int tag, unsigned int index) {
    totalLoads++;
    bool load = true;
    if (sets[index] == NULL) {
        sets[index] = new Set(isWriteAllocate, isWriteThrough, 
        lruOrFifo, numBlocks, numBytesPerBlock); 
    }
    
    Set* set = sets[index];
    cacheInfo * foundSlot = set->findSlotByTag(tag, load);
    if (foundSlot->hit == true) {
        loadTotalHits++;
        totalCycles++;
    }
    else {
        loadTotalMisses++;
    }

    totalCycles += (foundSlot->getTotalBytes() / 4) * 100;
}

void Cache::store(unsigned int address, unsigned int tag, unsigned int index) {
    totalStores++;
    bool load = false;

    if (sets[index] == NULL) {
        sets[index] = new Set(isWriteAllocate, isWriteThrough, 
        lruOrFifo, numBlocks, numBytesPerBlock); 
    }

    Set* set = sets[index];
    cacheInfo * foundSlot;
    foundSlot = set->findSlotByTag(tag, load);

    if (foundSlot->hit) {
        storeTotalHits++;
        totalCycles++;  // cost of accessing cache

        if (isWriteThrough) {
            // Write to main memory immediately
            totalCycles += (numBytesPerBlock / 4) * 100;
        } else {
            // Assume here we need a method to mark slot as dirty for write-back. 
            // Adjust this part based on your architecture
            Slot* slot = set->findActualSlotByTag(tag); // Assuming this method exists
            if (slot) slot->setDirty(true);
        }
    } else {
        storeTotalMisses++;
        if (isWriteAllocate) {
            foundSlot = set->addNewSlot(tag);
            foundSlot->bytesStored = numBytesPerBlock;
        } 
        else {
            // Directly write to memory if no write-allocate
            totalCycles += (numBytesPerBlock / 4) * 100;
        }
    }

    // delete foundSlot;
}

void Cache::printStatistics() const {
    cout << "Total loads: " << totalLoads << endl;
    cout << "Total stores: " << totalStores << endl;
    cout << "Load hits: " << loadTotalHits << endl;
    cout << "Load misses: " << loadTotalMisses << endl;
    cout << "Store hits: " << storeTotalHits << endl;
    cout << "Store misses: " << storeTotalMisses << endl;
    cout << "Total cycles: " << totalCycles << endl;
}
