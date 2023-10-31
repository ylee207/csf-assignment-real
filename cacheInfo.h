
#include <string>
#include <vector>
#include "Slot.h"
 
// mini class to determine result 
class cacheInfo {
    public:
    bool hit;
    unsigned bytesLoaded;
    unsigned bytesStored;
    int evictionCycles;

    // cacheInfo constructor
    cacheInfo() : hit(false), bytesLoaded(0), bytesStored(0), evictionCycles(0) {};
}; 