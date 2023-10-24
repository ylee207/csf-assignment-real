#include "Set.h"
#include <ctime>

Set::Set(bool isWriteAllocate, bool isWriteThrough, std::string& lruOrFifo, unsigned numBlocks, unsigned numBytesPerBlock) 
    : evictStrat(lruOrFifo), 
      writeAllocate(isWriteAllocate),
      writeThrough(isWriteThrough),
      maxBlocks(numBlocks),
      numBytes(numBytesPerBlock)
{
    
}

Set::~Set() {}

cacheInfo* Set::findSlotByTag(uint32_t tag, bool load) {
    cacheInfo * slotRes = new cacheInfo();
    slotRes->hit = false;
    slotRes->bytesLoaded = 0;
    slotRes->bytesStored = 0;

    unsigned count = 0;
    if (load) {
    for (Slot &slot : slots) {  //if it is hit
        if (slot.getTag() == tag && slot.isValid()) {
            slotRes->hit = true;
            slotRes->bytesLoaded = numBytes;
            return slotRes;
        }
        count++;
    }
    }
    if (!load) {
    for (Slot &slot : slots) {  //if it is hit
        if (slot.getTag() == tag && slot.isValid()) {
            slotRes->hit = true;
            slotRes->bytesLoaded = numBytes;
            return slotRes;
        }
        count++;
    }
    }

    //if it is miss
    if (count == maxBlocks) {
        evictSlot();
    }
    addNewSlot(tag);
    slotRes->bytesLoaded = numBytes;
    return slotRes;
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

    // Check if there's a slot available or we need to evict
    if (slots.size() < maxBlocks) {
        Slot newSlot;
        newSlot.setTag(tag);
        newSlot.setValid(true);
        newSlot.setLoadTimestamp(std::time(nullptr));
        newSlot.setAccessTimestamp(newSlot.getLoadTimestamp());
        slots.push_back(newSlot);

    } else {
        Slot *slotToReplace = evictSlot();
        slotToReplace->setTag(tag);
        slotToReplace->setValid(true);
        slotToReplace->setLoadTimestamp(std::time(nullptr));
        slotToReplace->setAccessTimestamp(slotToReplace->getLoadTimestamp());
    }

    return slotRes;
}

Slot* Set::evictSlot() {
    if (evictStrat == "lru") {
        Slot *lruSlot = &slots[0];
        for (Slot &slot : slots) {
            if (slot.getAccessTimestamp() < lruSlot->getAccessTimestamp()) {
                lruSlot = &slot;
            }
        }
        return lruSlot;
    } else { // Assuming "fifo"
        Slot *oldestSlot = &slots[0];
        for (Slot &slot : slots) {
            if (slot.getLoadTimestamp() < oldestSlot->getLoadTimestamp()) {
                oldestSlot = &slot;
            }
        }
        return oldestSlot;
    }
}