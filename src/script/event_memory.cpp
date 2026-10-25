// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptEvents.h"

void *BSEventGetEventMemoryBlock(unsigned int) {
    BSEventMemoryView *block = (BSEventMemoryView *)g_pMemBlockAllocator.GetFreeElement(true);
    block->referenceCount = 0;
    return block;
}
void BSEventCheckMemBlockReferenceCount(void *memory) {
    BSEventMemoryView *block = (BSEventMemoryView *)memory;
    if (block->referenceCount <= 0 && block->referenceCount != -65536) {
        block->referenceCount = -65536;
        g_pMemBlockAllocator.FreeElement(block);
    }
}
int BSEventIsEventMemoryBlock(void *memory) {
    return g_pMemBlockAllocator.DoWeOwnThisMemory(memory);
}
void BSEventCurrentlyProcessingMessages(bool processing) {
    if (processing) g_BSEventModule.processingState = 3;
    else g_BSEventModule.processingState = 0;
}
