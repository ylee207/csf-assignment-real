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

cacheInfo* Set::findSlotByTag(uint32_t tag, bool load) {
    cacheInfo * slotRes = new cacheInfo();
    slotRes->hit = false;
    slotRes->bytesLoaded = 0;
    slotRes->bytesStored = 0;
    currtimeStamp++;

    unsigned count = 0;

    if (load) {
        for (Slot &slot : slots) {  // if it is hit
            if (slot.getTag() == tag ) {
                slotRes->hit = true;
                slotRes->bytesLoaded = numBytes;
                slot.setAccessTimestamp(findMaxTime() + 1);
                return slotRes;
            }
            count++;
        }

        if (count == maxBlocks) {
            int extraCycles = evictSlot();
            slotRes->evictionCycles = extraCycles;
            addNewSlot(tag);
            return slotRes;
         } else {
            addNewSlot(tag);
            slotRes->bytesLoaded = numBytes;
            return slotRes;
         }

    }
    else {
        for (Slot &slot : slots) {  //if it is hit
            if (slot.getTag() == tag) {
                slotRes->hit = true;
                slotRes->bytesLoaded = numBytes;
                slot.setAccessTimestamp(findMaxTime() + 1);
                return slotRes;
            }
            count++;
        }

        if (count == maxBlocks) {
            int extraCycles = evictSlot();
            slotRes->evictionCycles = extraCycles;
            return slotRes;
         }
        else {
            //addNewSlot(tag);
            return slotRes;
        }
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
    slotRes->bytesLoaded = numBytes;

    Slot newSlot;
    newSlot.setTag(tag);
    newSlot.setValid(true);
    newSlot.setLoadTimestamp(currtimeStamp);
    newSlot.setAccessTimestamp(currtimeStamp);
    slots.push_back(newSlot);


    return slotRes;
}


int Set::evictSlot() {
    int extraCycles = 0;
    if (evictStrat == "lru") {
        std::vector<Slot>::iterator lruIter = slots.begin();
        for (std::vector<Slot>::iterator iter = slots.begin(); iter != slots.end(); ++iter) {
            if (iter->getAccessTimestamp() < lruIter->getAccessTimestamp()) {
                lruIter = iter;
            }
        }

        if (lruIter->isDirty()) {
            extraCycles = (numBytes / 4) * 100;
        }

        slots.erase(lruIter);
    } else { // Assuming "fifo"
        std::vector<Slot>::iterator oldestIter = slots.begin();
        for (std::vector<Slot>::iterator iter = slots.begin(); iter != slots.end(); ++iter) {
            if (iter->getLoadTimestamp() < oldestIter->getLoadTimestamp()) {
                oldestIter = iter;
            }
        }

        if (oldestIter->isDirty()) {
            extraCycles = (numBytes / 4) * 100;
        }

        slots.erase(oldestIter);
    }
    return extraCycles;
}
