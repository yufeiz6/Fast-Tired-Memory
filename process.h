#ifndef PROCESS_H
#define PROCESS_H

#include "TwoLevelPageTable.h"
#include <cstdint>

class process {
public:
    long int pid;
    long int size;
    long int heapPages;
    uint32_t code;
    uint32_t stack;
    uint32_t heap;
    TwoLevelPageTable pageTable;
    process(long int pidGiven);
    map<uint32_t, map<uint32_t, uint32_t>> hugePageSegmentAccessMap;
    void allocateMem(uint32_t allocatedSize);
    void freeMem(uint32_t freedSize);
    uint32_t getHeap();
    //map<uint32_t, uint32_t> getHugePageAccessMap();
};

#endif