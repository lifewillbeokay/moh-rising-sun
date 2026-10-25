// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptEvents.h"

void BSEventIncrimentReferenceCount(void *memory) {
    ++((BSEventMemoryView *)memory)->referenceCount;
}
void BSEventDecrimentReferenceCount(void *memory) {
    BSEventMemoryView *block = (BSEventMemoryView *)memory;
    // The original checks the sentinel after decrementing.
    --block->referenceCount;
    if (block->referenceCount <= 0 && block->referenceCount != -65536) {
        block->referenceCount = -65536;
        g_pMemBlockAllocator.FreeElement(block);
    }
}
