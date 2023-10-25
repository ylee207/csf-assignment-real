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


cacheInfo* Set::findSlotByTag(uint32_t tag, bool load) {
    cacheInfo * slotRes = new cacheInfo();
    slotRes->hit = false;
    slotRes->bytesLoaded = 0;
    slotRes->bytesStored = 0;
    currtimeStamp++;

    unsigned count = 0;
    if (load) {
        for (Slot &slot : slots) {  //if it is hit
            if (slot.getTag() == tag ) {
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
        if (count == maxBlocks) {
            evictSlot();
            addNewSlot(tag);
            return slotRes;
         } else {
            addNewSlot(tag);
            slotRes->bytesLoaded = numBytes;
            return slotRes;
         }

    }
    if (!load) {
        for (Slot &slot : slots) {  //if it is hit
            if (slot.getTag() == tag) {
                slotRes->hit = true;
                slotRes->bytesLoaded = numBytes;
                return slotRes;
            }
            count++;
        }

        if (count == maxBlocks) {
            evictSlot();
            return slotRes;
         }
        else {
            //addNewSlot(tag);
            return slotRes;
        }
    }

    //if it is miss
    
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
        newSlot.setLoadTimestamp(currtimeStamp);
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

void Set::evictSlot() {
    if (evictStrat == "lru") {
        Slot *lruSlot = &slots[0];
        for (Slot slot : slots) {
            if (slot.getAccessTimestamp() < lruSlot->getAccessTimestamp()) {
                lruSlot = &slot;
            }
        }
        for (long unsigned int i = 0; i < slots.size(); i++) {
            if (slots[i].getTag() == lruSlot->getTag()) {
                slots.erase(slots.begin() + i);
                break;
            }
        }
    } else { // Assuming "fifo"
        Slot *oldestSlot = &slots[0];
        for (Slot slot : slots) {
            if (slot.getLoadTimestamp() < oldestSlot->getLoadTimestamp()) {
                oldestSlot = &slot;
            }
        }
        for (long unsigned int i = 0; i < slots.size(); i++) {
            if (slots[i].getTag() == oldestSlot->getTag()) {
                slots.erase(slots.begin() + i);
                break;
            }
        }

    }
}
