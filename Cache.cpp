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
    cacheInfo * foundSlot = set->findSlotByTag(tag, load);
    // totalCycles += foundSlot->evictionCycles;
    if (foundSlot->hit == true) { // hit
        loadTotalHits++;
    }
    else { // miss
        loadTotalMisses++;
        totalCycles += (foundSlot->getTotalBytes() / 4) * 100;
    }
    totalCycles++;
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
    foundSlot = set->findSlotByTag(tag, load);
    totalCycles += foundSlot->evictionCycles;

    // If it's a hit
    if (foundSlot->hit) {
        storeTotalHits++;

        // write-through
        if (isWriteThrough) {
            // Write to main memory immediately
            totalCycles += (numBytesPerBlock / 4) * 100;
            // totalCycles += 100;
            totalCycles++;
        } else {
            // write-back
            Slot* slot = set->findActualSlotByTag(tag);
            if (slot) slot->setDirty(true);
            totalCycles++;
        }
    // If it's a miss
    } else {
        storeTotalMisses++;
        if (isWriteAllocate) {
            totalCycles += (numBytesPerBlock / 4) * 100;
            foundSlot = set->addNewSlot(tag);
            foundSlot->bytesStored = numBytesPerBlock;
            totalCycles++;
            totalCycles += (numBytesPerBlock / 4) * 100;
            totalCycles++;
            Slot* slot = set->findActualSlotByTag(tag);
            if (slot) slot->setDirty(true);

            if (isWriteThrough) {
                totalCycles += (numBytesPerBlock / 4) * 100 + 1;
            }
        }
        else {
            // Directly write to memory if no write-allocate
            // Slot* slot = set->findActualSlotByTag(tag);
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
