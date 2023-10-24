#ifndef SLOT_H
#define SLOT_H

#include <cstdint>

class Slot {
public:
    Slot();
    ~Slot();

    uint32_t getTag() const;
    void setTag(uint32_t newTag);

    bool isValid() const;
    void setValid(bool validStatus);

    uint32_t getLoadTimestamp() const;
    void setLoadTimestamp(uint32_t timestamp);

    uint32_t getAccessTimestamp() const;
    void setAccessTimestamp(uint32_t timestamp);

    bool isDirty() const;
    void setDirty(bool dirtyStatus);

private:
    uint32_t tag;
    bool valid;
    uint32_t load_ts;
    uint32_t access_ts;
    bool dirty;
};

#endif