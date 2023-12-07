#include "os.h"
#include "tlb.h"
#include "TwoLevelPageTable.h"
#include <stdint.h>
#include <fstream>
#include <sstream>

int main(int argc, char *argv[]) {
    size_t memorySize = 1ULL << 32; 
    size_t diskSize = 1024 * 1024 * 1024 * 10; 
    uint32_t high_watermark = 200 * 1024 * 1024;
    uint32_t low_watermark = 100 * 1024 * 1024;

    os osInstance(memorySize, diskSize, high_watermark, low_watermark);
    
    ifstream inputFile(argv[1]);
    if (!inputFile) {
        cerr << "Error: Unable to open file." << endl;
        return 1;
    }

    string line;
    bool switchExecuted = false;
    while (getline(inputFile, line)) {
        //cout << "Executing command: " << line << endl;
        istringstream iss(line);
        uint32_t pid;
        string instruction;
        uint32_t value = 0;

        iss >> pid >> instruction;
        if (instruction == "switch") {
            if (switchExecuted) {
                // If a switch has already been executed, break out of the loop
                break;
            }
            osInstance.switchToProcess(pid);
            auto& segmentAccessMap = osInstance.runningProc->hugePageSegmentAccessMap;
            auto& pageSizeToSegmentCountMap = osInstance.pageSizeToSegmentCountMap;
            for (const auto& hugePageEntry : segmentAccessMap) {
                  cout << "Huge Page PFN: " << hugePageEntry.first << endl;

                  uint32_t numSegments = pageSizeToSegmentCountMap[hugePageEntry.first];
                  cout << "  Total 4KB segments in this huge page: " << numSegments << endl;
                  for (const auto& segmentEntry : hugePageEntry.second) {
                      cout << "  4KB Segment Offset: " << segmentEntry.first 
                          << ", Access Count: " << segmentEntry.second << endl;
                  }
              }
        } else {
            if (!(iss >> hex >> value)) {
                cerr << "Error parsing value for instruction: " << instruction << endl;
                continue;
            }
            osInstance.handleInstruction(instruction, value, pid);
        }
    }

    cout << "Cache Hits: " << osInstance.cacheHit << endl;
    cout << "Cache Misses: " << osInstance.cacheMiss << endl;
    if (osInstance.cacheHit + osInstance.cacheMiss > 0) {
        double hitRate = static_cast<double>(osInstance.cacheHit) / 
                        (osInstance.cacheHit + osInstance.cacheMiss);
        cout << "Cache Hit Rate: " << hitRate << endl;
    }
   
    cout << "Total memory access attempts: " << memory_access_attempts << endl;
    inputFile.close();

    /*
    cout << "OS initialized" << endl;
    uint32_t pid = osInstance.createProcess(1);
    osInstance.switchToProcess(pid);
    cout << "before memory allocated" << osInstance.runningProc->heap<< endl;
    osInstance.allocateMemory(16384);
    auto& accessMap = osInstance.runningProc->hugePageAccessMap;

    cout << "after memory allocated" << osInstance.runningProc->heap<< endl;

    for (int i = 0; i < 10; i++) {
      osInstance.accessHeap(4096 + i * 1024); // Access different parts of the large page
    }

    osInstance.allocateMemory(16384);

    for (int i = 0; i < 10; i++) {
      osInstance.accessHeap(16384 + i * 1024); // Access different parts of the large page
    }

    for (const auto& entry : accessMap) {
    cout << "PFN: " << entry.first << ", Access Count: " << entry.second << endl;
    }
    */

    return 0;
}