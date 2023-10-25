#include "Set.h"
#include <cstdint>
#include <ctime>

Set::Set(bool isWriteAllocate, bool isWriteThrough, std::string& lruOrFifo, unsigned numBlocks, unsigned numBytesPerBlock) 
    : evictStrat(lruOrFifo), 
      writeAllocate(isWriteAllocate),
      writeThrough(isWriteThrough),
      maxBlocks(numBlocks),
      numBytes(numBytesPerBlock),
      currtimeStamp(0)
{
    
}

Set::~Set() {}


cacheInfo* Set::findSlotByTag(uint32_t tag, bool load) {
    cacheInfo * slotRes = new cacheInfo();
    slotRes->hit = false;
    slotRes->bytesLoaded = 0;
    slotRes->bytesStored = 0;
    currtimeStamp++;

    unsigned count = 0;
    if (load) {
        for (Slot &slot : slots) {  //if it is hit
            if (slot.getTag() == tag && slot.isValid()) {
                slotRes->hit = true;
                slotRes->bytesLoaded = numBytes;
                // int prevTime = slot.getAccessTimestamp();
                // slot.setAccessTimestamp(0);
                // for (int i = 0; i < slots.size(); i++) {
                //     if (slots[i].getAccessTimestamp() < prevTime) {
                //         slots[i].setAccessTimestamp(slots[i].getAccessTimestamp() + 1);
                //     }
                // }
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
        slotRes->hit = false;
        evictSlot(tag);
        return slotRes;

    }
    else {
        slotRes->hit = false;
        addNewSlot(tag);
        slotRes->bytesLoaded = numBytes;
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
    slotRes->bytesLoaded = numBytes;

    // Check if there's a slot available or we need to evict
    // if (slots.size() < maxBlocks) {
        Slot newSlot;
        newSlot.setTag(tag);
        newSlot.setValid(true);
        newSlot.setLoadTimestamp(currtimeStamp++);
        //newSlot.setAccessTimestamp();
        slots.push_back(newSlot);

    // } else {
    //     Slot *slotToReplace = evictSlot();
    //     slotToReplace->setTag(tag);
    //     slotToReplace->setValid(true);
    //     slotToReplace->setLoadTimestamp(currtimeStamp++);
    //     slotToReplace->setAccessTimestamp(slotToReplace->getLoadTimestamp());
    // }

    return slotRes;
}

void Set::evictSlot(uint32_t tag) {
    if (evictStrat == "lru") {
        Slot *lruSlot = &slots[0];
        for (Slot slot : slots) {
            if (slot.getAccessTimestamp() < lruSlot->getAccessTimestamp()) {
                lruSlot = &slot;
            }
        }
        for (int i = 0; i < slots.size(); i++) {
            if (slots[i].getTag() == lruSlot->getTag()) {
                slots.erase(slots.begin() + i);
                break;
            }
        }
    } else { // Assuming "fifo"
        Slot *oldestSlot = &slots[0];
        for (Slot slot : slots) {
            if (slot.getLoadTimestamp() > oldestSlot->getLoadTimestamp()) {
                oldestSlot = &slot;
            }
        }
        oldestSlot->setTag(tag);
        oldestSlot->setValid(true);
        oldestSlot->setLoadTimestamp(currtimeStamp);

    }
}
