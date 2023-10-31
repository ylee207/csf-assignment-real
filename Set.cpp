#include "Set.h"
#include <cstdint>
#include <ctime>

Set::Set(bool isWriteAllocate, bool isWriteThrough, std::string& lruOrFifo, unsigned numBlocks, unsigned numBytesPerBlock) 
    : writeAllocate(isWriteAllocate),
      writeThrough(isWriteThrough),
      evictStrat(lruOrFifo),
      maxBlocks(numBlocks),
      numBytes(numBytesPerBlock),
      currtimeStamp(0)
{
    
}

Set::~Set() {}

int Set::findMaxTime() {
    Slot maxSlot = slots[0];
    for (Slot slot : slots) {
        if (slot.getAccessTimestamp() > maxSlot.getAccessTimestamp()) {
            maxSlot = slot;
        }
    }
    return maxSlot.getAccessTimestamp();
}

cacheInfo* Set::findSlotByTag(uint32_t tag, bool load, bool writeThrough, bool writeAllocate, unsigned * tc) {
    cacheInfo * slotRes = new cacheInfo();
    slotRes->hit = false;
    slotRes->bytesLoaded = 0;
    slotRes->bytesStored = 0;
    currtimeStamp++;

    unsigned count = 0;

    if (load) {
        for (Slot &slot : slots) { 
            // if it is hit
            if (slot.getTag() == tag && slot.isValid()) {
                *tc += 1;
                slotRes->hit = true;
                slot.setAccessTimestamp(currtimeStamp++);
                return slotRes;
            }
            count++;
        }

        // if miss
        if (count == maxBlocks) {
            evictSlot(tc);
            addNewSlot(tag);
            *tc += (numBytes / 4) * 100 + 1;
            return slotRes;
         } else {
            addNewSlot(tag);
            *tc += (numBytes / 4) * 100 + 1;
            return slotRes;
         }
    }
    // store
    else {
        for (Slot &slot : slots) {
            // if it is hit
            if (slot.getTag() == tag && slot.isValid()) {
                if (!writeThrough) {
                    slot.setDirty(true);
                    *tc += 1;
                }
                if (writeThrough) {
                    *tc += 100 + 1;
                }

                slotRes->hit = true;
                slot.setAccessTimestamp(currtimeStamp++);
                return slotRes;
            }
            count++;
        }

        if (writeThrough) {
            slotRes->hit = false;
            *tc += 100;
        }
        if (writeAllocate) { 
            if (count == maxBlocks) {
                evictSlot(tc);
                addNewSlot(tag);
                *tc += (numBytes / 4) * 100 + 1;
                return slotRes;
            } else {
                addNewSlot(tag);
                *tc += (numBytes / 4) * 100 + 1;
                return slotRes;
            }
        }

        // if (count == maxBlocks) {
        //     bool isDirty = evictSlot(tc);
        //     // if (writeThrough) {
        //     //     // Add for writeThrough?
        //     // } else {
        //     //     if (isDirty) {
        //     //         *tc += (numBytes / 4) * 100;
        //     //     }
        //     // }
        //     return slotRes;
        //  }
        // else {
        //     // Fix this part.
        //     if (writeThrough) {
        //         slotRes->bytesLoaded = numBytes;
        //         slotRes->bytesStored = 4;
        //     } else {
        //         slotRes->bytesLoaded = numBytes;
        //         slotRes->bytesStored = 0;
        //     }
        //     return slotRes;
        // }
        return slotRes;
    }
}

Slot* Set::findActualSlotByTag(uint32_t tag) {
    for (Slot &slot : slots) {
        if (slot.getTag() == tag && slot.isValid()) {
            return &slot; // Return the address of the found slot
        }
    }
    return nullptr; // If no matching slot found
}


cacheInfo * Set::addNewSlot(uint32_t tag) {
    cacheInfo * slotRes = new cacheInfo();
    slotRes->hit = false;

    Slot newSlot;
    newSlot.setTag(tag);
    newSlot.setValid(true);
    newSlot.setLoadTimestamp(currtimeStamp);
    newSlot.setAccessTimestamp(currtimeStamp);
    slots.push_back(newSlot);


    return slotRes;
}


bool Set::evictSlot(unsigned *tc) {
    // int extraCycles = 0;
    if (evictStrat == "lru") {
        std::vector<Slot>::iterator lruIter = slots.begin();
        for (std::vector<Slot>::iterator iter = slots.begin(); iter != slots.end(); ++iter) {
            if (iter->getAccessTimestamp() < lruIter->getAccessTimestamp()) {
                lruIter = iter;
            }
        }

        if (!writeThrough && lruIter->isDirty()) {
            *tc += (numBytes / 4) * 100;
        }

        slots.erase(lruIter);
        return lruIter->isDirty();
    } else { // Assuming "fifo"
        std::vector<Slot>::iterator oldestIter = slots.begin();
        for (std::vector<Slot>::iterator iter = slots.begin(); iter != slots.end(); ++iter) {
            if (iter->getLoadTimestamp() < oldestIter->getLoadTimestamp()) {
                oldestIter = iter;
            }
        }
        if (!writeThrough && oldestIter->isDirty()) {
            *tc += (numBytes / 4) * 100;
        }

        slots.erase(oldestIter);
        return oldestIter->isDirty();
    }
}