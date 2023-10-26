
#include <string>
#include <vector>
#include "Slot.h"
 
class cacheInfo {   //mini class to determine result 
    public:
    bool hit;
    unsigned bytesLoaded;
    unsigned bytesStored;
    int evictionCycles;

    cacheInfo() : hit(false), bytesLoaded(0), bytesStored(0), evictionCycles(0) {};

    unsigned getTotalBytes() {
        return bytesLoaded + bytesStored;
    }
}; 