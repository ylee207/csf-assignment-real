#include "Slot.h"

Slot::Slot() : tag(0), valid(false), load_ts(0), access_ts(0), dirty(false) { }

Slot::~Slot() { }

uint32_t Slot::getTag() const { 
    return tag; 
}

void Slot::setTag(uint32_t newTag) {
    tag = newTag;
}

bool Slot::isValid() const {
    return valid;
}

void Slot::setValid(bool validStatus) {
    valid = validStatus;
}

uint32_t Slot::getLoadTimestamp() const {
    return load_ts;
}

void Slot::setLoadTimestamp(uint32_t timestamp) {
    load_ts = timestamp;
}

uint32_t Slot::getAccessTimestamp() const {
    return access_ts;
}

void Slot::setAccessTimestamp(uint32_t timestamp) {
    access_ts = timestamp;
}

bool Slot::isDirty() const {
    return dirty;
}

void Slot::setDirty(bool dirtyStatus) {
    dirty = dirtyStatus;
}
