// AI-assisted partial reconstruction; see docs/research/broad-source-sweep.md.
#include "MemoryCardDevice.h"
#ifdef DECOMP_COMPARE
void CMCDevice::Cache(bool inserted) {
    justInserted = inserted;
    g_hMemoryCard->Cache((CMemoryCard::Device *)this);
}
#endif
