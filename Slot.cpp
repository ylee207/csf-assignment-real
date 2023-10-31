#include "Slot.h"

// Default constructor: Initializes a slot with default values.
Slot::Slot() : tag(0), valid(false), load_ts(0), access_ts(0), dirty(false) { }

// Default destructor for the Slot class. Currently does nothing.
Slot::~Slot() { }

// Returns the tag of the slot.
uint32_t Slot::getTag() const { 
    return tag; 
}

// Sets a new tag for the slot.
void Slot::setTag(uint32_t newTag) {
    tag = newTag;
}

// Returns the validity status of the slot (true if valid, false otherwise).
bool Slot::isValid() const {
    return valid;
}

// Sets the validity status for the slot.
void Slot::setValid(bool validStatus) {
    valid = validStatus;
}

// Returns the timestamp when the slot was loaded/created.
uint32_t Slot::getLoadTimestamp() const {
    return load_ts;
}

// Sets a new timestamp for when the slot was loaded/created.
void Slot::setLoadTimestamp(uint32_t timestamp) {
    load_ts = timestamp;
}

// Returns the timestamp of the last access to the slot.
uint32_t Slot::getAccessTimestamp() const {
    return access_ts;
}

// Sets a new timestamp for the last access to the slot.
void Slot::setAccessTimestamp(uint32_t timestamp) {
    access_ts = timestamp;
}

// Returns the dirty status of the slot (true if dirty, false otherwise).
bool Slot::isDirty() const {
    return dirty;
}

// Sets the dirty status for the slot.
void Slot::setDirty(bool dirtyStatus) {
    dirty = dirtyStatus;
}
