#ifndef SET_H
#define SET_H

#include <cstdint>
#include <string>
#include <vector>
#include "Slot.h"
#include "cacheInfo.h"

// The Set class represents a set in a cache, which may contain multiple slots/blocks.
class Set {
public:
    /*
     * Constructor: Initializes a Set with the given properties.
     * - isWriteAllocate: Determines if cache employs write-allocate policy.
     * - isWriteThrough: Determines if cache employs write-through policy.
     * - lruOrFifo: Eviction strategy for the set, either "lru" or "fifo".
     * - numBlocks: Number of blocks (slots) in the set.
     * - numBytesPerBlock: Number of bytes per block.
     */
    Set(bool isWriteAllocate, bool isWriteThrough, std::string& lruOrFifo, unsigned numBlocks, unsigned numBytesPerBlock); 
    
    // Set Destructor
    ~Set();

    /*
     * Searches for a slot in the set by the given tag and updates the hit/miss statistics.
     * Returns information about the cache operation (hit/miss, bytes loaded/stored).
     */
    cacheInfo * findSlotByTag(uint32_t tag, bool load, bool writeThrough, bool writeAllocate, unsigned * tc);
    
    /*
     * Returns a pointer to an actual slot in the set by the given tag or nullptr if not found.
     */
    Slot* findActualSlotByTag(uint32_t tag);

    /*
     * Adds a new slot to the set with the given tag and returns cache operation information.
     */
    cacheInfo * addNewSlot(uint32_t tag);

    /*
     * Evicts a slot from the set based on the chosen eviction strategy (LRU/FIFO).
     * Returns true if evicted slot was dirty, otherwise false.
     */
    bool evictSlot(unsigned *tc);  // LRU or FIFO depending on what you choose
    
    /*
     * Finds and returns the maximum timestamp among all slots in the set. Used for LRU eviction.
     */
    int findMaxTime();

    /*
     * Vector containing all the slots in this set.
     */
    std::vector<Slot> slots;

private:
    bool writeAllocate; // Determines if the cache uses the write-allocate policy.
    bool writeThrough; // Determines if the cache uses the write-through policy.
    std::string& evictStrat; // The eviction strategy used by the set ("lru" or "fifo").
    unsigned maxBlocks; // The maximum number of blocks (slots) in the set.
    unsigned numBytes; // Number of bytes per block in the set.
    unsigned currtimeStamp; // Tracks the current time or access order for the LRU/FIFO policies.
};

#endif
