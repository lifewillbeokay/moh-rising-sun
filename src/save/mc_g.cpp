// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic reference.
// Part of the original MemoryCard.cpp.
#include "MemoryCard.h"

extern void *g_pTextDatabase;
extern void *g_pMiscTextDatabase;
extern void *g_pScratchpad;
extern int g_bWaitingForReply;
extern int g_bIsBusy;
extern "C" int MEM_free(void *);

void CMemoryCard::TellTheGamecubeToShutUp(bool) {
}

int CMemoryCard::WasDataWritten() const {
    return ((CMemoryCard *)this)->SaveSuccessful();
}
