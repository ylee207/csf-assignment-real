#ifndef SLOT_H
#define SLOT_H

#include <cstdint>

// Class representing an individual slot within the cache.
class Slot {
public:
    // Default constructor
    Slot();

    // Destructor
    ~Slot();

    // Returns the tag associated with this slot.
    uint32_t getTag() const;
    // Sets the tag for this slot.
    void setTag(uint32_t newTag);

    // Checks if the slot is currently valid (e.g., contains valid data).
    bool isValid() const;
    // Sets the validity status of the slot.
    void setValid(bool validStatus);

    // Retrieves the timestamp of when the slot was loaded/created.
    uint32_t getLoadTimestamp() const;
    // Sets the timestamp indicating when the slot was loaded/created.
    void setLoadTimestamp(uint32_t timestamp);

    // Retrieves the timestamp of the most recent access to this slot.
    uint32_t getAccessTimestamp() const;
    // Sets the timestamp of the most recent access to this slot.
    void setAccessTimestamp(uint32_t timestamp);

    // Checks if the slot is dirty (e.g., has been modified but not written back).
    bool isDirty() const;
    // Sets the dirty status of the slot.
    void setDirty(bool dirtyStatus);

private:
    uint32_t tag;       // The identifier for data stored in this slot.
    bool valid;         // Flag to indicate if the data in the slot is valid.
    uint32_t load_ts;   // Timestamp for when the slot was loaded/created.
    uint32_t access_ts; // Timestamp for the most recent access to the slot.
    bool dirty;         // Flag to indicate if the slot contains modified data that hasn't been written back.
};

#endif