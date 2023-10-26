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

void Cache::load(unsigned int tag, unsigned int index) {
    totalLoads++;
    bool load = true;
    if (sets[index] == nullptr) {
        sets[index] = new Set(isWriteAllocate, isWriteThrough, 
        lruOrFifo, numBlocks, numBytesPerBlock); 
    }
    
    Set* set = sets[index];
    cacheInfo * foundSlot = set->findSlotByTag(tag, load, isWriteThrough, isWriteAllocate, &totalCycles);
    if (foundSlot->hit == true) { // hit
        loadTotalHits++;
    }
    else { // miss
        loadTotalMisses++;
    }
}

void Cache::store(unsigned int tag, unsigned int index) {
    totalStores++;
    bool load = false;

    if (sets[index] == nullptr) {
        sets[index] = new Set(isWriteAllocate, isWriteThrough, 
        lruOrFifo, numBlocks, numBytesPerBlock); 
    }

    Set* set = sets[index];
    cacheInfo * foundSlot;
    foundSlot = set->findSlotByTag(tag, load, isWriteThrough, isWriteAllocate, &totalCycles);

    // If it's a hit
    if (foundSlot->hit) {
        storeTotalHits++;

        // write-through
        if (isWriteThrough) {
            // Write to main memory immediately
        } else {
            // write-back
            Slot* slot = set->findActualSlotByTag(tag);
            if (slot) slot->setDirty(true);
        }
    // If it's a miss
    } else {
        storeTotalMisses++;
        if (isWriteAllocate) {
            foundSlot = set->addNewSlot(tag);
            Slot* slot = set->findActualSlotByTag(tag);
            if (slot) slot->setDirty(true);
        }
    }

    delete foundSlot;
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