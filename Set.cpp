#include "Set.h"
#include <cstdint>
#include <ctime>

// Constructor for the Set class, initialized with cache set properties and options.
Set::Set(bool isWriteAllocate, bool isWriteThrough, std::string& lruOrFifo, unsigned numBlocks, unsigned numBytesPerBlock) 
    : writeAllocate(isWriteAllocate),
      writeThrough(isWriteThrough),
      evictStrat(lruOrFifo),
      maxBlocks(numBlocks),
      numBytes(numBytesPerBlock),
      currtimeStamp(0)
{}

// Destructor for the Set class.
Set::~Set() {}

// Function to find the maximum timestamp, which can be used to implement LRU policy.
int Set::findMaxTime() {
    Slot maxSlot = slots[0];
    for (Slot slot : slots) {
        if (slot.getAccessTimestamp() > maxSlot.getAccessTimestamp()) {
            maxSlot = slot;
        }
    }
    return maxSlot.getAccessTimestamp();
}

// Function to find a slot based on the given tag. It also takes into consideration load/store operations and cache policies.
cacheInfo* Set::findSlotByTag(uint32_t tag, bool load, bool writeThrough, bool writeAllocate, unsigned * tc) {
    cacheInfo * slotRes = new cacheInfo();
    slotRes->hit = false;
    slotRes->bytesLoaded = 0;
    slotRes->bytesStored = 0;
    currtimeStamp++;

    unsigned count = 0;

    // Handling load operations.
    if (load) {
        for (Slot &slot : slots) { 
            // If it's a cache hit.
            if (slot.getTag() == tag && slot.isValid()) {
                *tc += 1;
                slotRes->hit = true;
                slot.setAccessTimestamp(currtimeStamp++);
                return slotRes;
            }
            count++;
        }

        // Handling cache miss scenario.
        if (count == maxBlocks) {
            evictSlot(tc);
            addNewSlot(tag);
            *tc += (numBytes / 4) * 100 + 1; // Account for load miss penalty.
            return slotRes;
         } else {
            addNewSlot(tag);
            *tc += (numBytes / 4) * 100 + 1; // Account for load miss penalty.
            return slotRes;
         }
    }
    // Handling store operations.
    else {
        for (Slot &slot : slots) {
            // If it's a cache hit.
            if (slot.getTag() == tag && slot.isValid()) {
                // Check cache write policies.
                if (!writeThrough) {
                    slot.setDirty(true);
                    *tc += 1; // Account for store hit penalty.
                }
                if (writeThrough) {
                    *tc += 100 + 1; // Account for write-through penalty.
                }

                slotRes->hit = true;
                slot.setAccessTimestamp(currtimeStamp++);
                return slotRes;
            }
            count++;
        }

        // Handling store miss scenarios.
        if (writeThrough) {
            slotRes->hit = false;
            *tc += 100; // Account for store miss penalty.
        }
        if (writeAllocate) { 
            // Evict a slot if necessary.
            if (count == maxBlocks) {
                evictSlot(tc);
                addNewSlot(tag);
                *tc += (numBytes / 4) * 100 + 1; // Account for store miss penalty.
                return slotRes;
            } else {
                addNewSlot(tag);
                *tc += (numBytes / 4) * 100 + 1; // Account for store miss penalty.
                return slotRes;
            }
        }

        return slotRes;
    }
}

// Function to return the actual slot object based on the given tag.
Slot* Set::findActualSlotByTag(uint32_t tag) {
    for (Slot &slot : slots) {
        if (slot.getTag() == tag && slot.isValid()) {
            return &slot; // Return the address of the found slot.
        }
    }
    // If no matching slot found
    return nullptr;
}

// Function to add a new slot with the given tag to the set.
cacheInfo * Set::addNewSlot(uint32_t tag) {
    cacheInfo * slotRes = new cacheInfo();
    slotRes->hit = false;

    Slot newSlot;
    newSlot.setTag(tag);
    newSlot.setValid(true);
    newSlot.setLoadTimestamp(currtimeStamp);
    newSlot.setAccessTimestamp(currtimeStamp);
    slots.push_back(newSlot); // Append the newly created slot to the set.

    return slotRes;
}

// Function to evict a slot from the set based on the eviction strategy (LRU/FIFO).
bool Set::evictSlot(unsigned *tc) {
    // Implementing LRU eviction strategy.
    if (evictStrat == "lru") {
        std::vector<Slot>::iterator lruIter = slots.begin();
        for (std::vector<Slot>::iterator iter = slots.begin(); iter != slots.end(); ++iter) {
            if (iter->getAccessTimestamp() < lruIter->getAccessTimestamp()) {
                lruIter = iter; // Find the least recently used slot.
            }
        }

        // Check dirty bit for write-back policy.
        if (!writeThrough && lruIter->isDirty()) {
            *tc += (numBytes / 4) * 100; // Account for write-back penalty.
        }

        slots.erase(lruIter);
        return lruIter->isDirty();
    } 
    // Implementing FIFO eviction strategy.
    else {
        std::vector<Slot>::iterator oldestIter = slots.begin();
        for (std::vector<Slot>::iterator iter = slots.begin(); iter != slots.end(); ++iter) {
            if (iter->getLoadTimestamp() < oldestIter->getLoadTimestamp()) {
                oldestIter = iter; // Find the oldest slot.
            }
        }

        // Check dirty bit for write-back policy.
        if (!writeThrough && oldestIter->isDirty()) {
            *tc += (numBytes / 4) * 100; // Account for write-back penalty.
        }

        slots.erase(oldestIter);
        return oldestIter->isDirty();
    }
}