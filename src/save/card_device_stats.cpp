// AI-assisted reconstruction from GR8E69; see docs/research/broad-source-sweep.md.
#include "MemoryCardDevice.h"
void CMCDevice::GetStats(unsigned int &free, unsigned int &total) const {
    free = freeBlocks;
    total = totalBlocks;
}
int CMCDevice::GetStatus() const { return status; }
