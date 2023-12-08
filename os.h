#ifndef OS_H
#define OS_H

#include "TwoLevelPageTable.h"
#include "process.h"
#include "tlb.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <map>
#include <stdexcept>
using namespace std;

extern int memory_access_attempts;
extern int stack_miss;
extern int heap_miss;
extern int code_miss;

/* support 4kb subpages
struct CacheKey {
    uint32_t pfn;
    uint32_t offset;

    CacheKey(uint32_t pfn, uint32_t offset) : pfn(pfn), offset(offset) {}

    bool operator<(const CacheKey& other) const {
        return std::tie(pfn, offset) < std::tie(other.pfn, other.offset);
    }
};
*/

struct CacheKey {
    uint32_t pfn;

    explicit CacheKey(uint32_t pfn) : pfn(pfn) {}

    bool operator<(const CacheKey& other) const {
        return pfn < other.pfn;
    }
};

class os {
private:
    int minPageSize;
    //process* runningProc;
    uint32_t HUGE_PAGE_SIZE = 128 * 4096;
    uint32_t Cache_Size = 512;
    vector<bool> memoryMap;
    vector<process> processes;
    vector<bool> diskMap;
    uint32_t high_watermark;
    uint32_t low_watermark;
    uint32_t totalFreeSize;
    //std::vector<uint32_t> disk;
    map<uint32_t, uint32_t> pageToDiskMap;
    Tlb tlb;


public:
    os(size_t memorySize, size_t diskSize, uint32_t high_watermarkGiven, uint32_t low_watermarkGiven);
    ~os();
    process* runningProc;
    uint32_t cacheHit;
    uint32_t cacheMiss;
    map<uint32_t, map<uint32_t, uint32_t>> hugePageSegmentAccessMap;
    map<uint32_t, uint32_t> pageSizeToSegmentCountMap; //stores the pfn of the huge page to number of 4kb subpages in it.
    map<CacheKey, uint32_t> cache; //stores pfn & offset to freq so we know which 4kb segment it is
    uint32_t allocateMemory(uint32_t size);
    void freeMemory(uint32_t baseAddress);
    uint32_t createProcess(long int pid);
    void accessCache(const CacheKey& key);
    //void destroyProcess(long int pid);
    void swapOutToMeetWatermark(uint32_t sizeTobeFree);
    void swapOutPage(uint32_t vpn, uint32_t pfn);
    uint32_t swapInPage(uint32_t vpn, uint32_t size);
    uint32_t findFreeFrame();
    void handleInstruction(const string& string, uint32_t value, uint32_t pid);
    uint32_t accessStack(uint32_t baseAddress);
    uint32_t accessHeap(uint32_t baseAddress);
    uint32_t accessCode(uint32_t baseAddress);
    void accessMemory(uint32_t baseAddress);
    void switchToProcess(uint32_t pid);
    vector<pair<uint32_t, uint32_t> > findPhysicalFrames(uint32_t size);
    uint32_t findFreeDiskBlock();
};

#endif // OS_H
