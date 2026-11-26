// AI-assisted GR8E69 reconstruction; see docs/research/save-popups.md.
#include "MemoryCardGC.h"
CMemoryCardGC::~CMemoryCardGC() { interface->Release(); g_pMCInstance = 0; }
