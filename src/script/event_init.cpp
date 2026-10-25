// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptEvents.h"

int BSEventGetMemoryRequirements() {
    return 16 * sizeof(BSEventBucketView) + 256 * sizeof(BSEventRecord_struct)
        + g_pMemBlockAllocator.ComputeNeededSize(sizeof(BSEventMemoryView), 128, 0);
}

int BSInitEvent() {
    DWI_charfill(&g_BSEventModule, sizeof(g_BSEventModule), 0);
    g_BSEventModule.memory = (unsigned char *)BSUtilGetMemory(BSEventGetMemoryRequirements(), 0);
    g_BSEventModule.buckets = (BSEventBucketView *)g_BSEventModule.memory;
    g_BSEventModule.memoryOffset = 16 * sizeof(BSEventBucketView);
    for (int i = 0; i < 16; ++i) {
        g_BSEventModule.buckets[i].head = 0;
        g_BSEventModule.buckets[i].tail = 0;
    }
    g_BSEventModule.records = (BSEventRecord_struct *)(g_BSEventModule.memory + g_BSEventModule.memoryOffset);
    g_BSEventModule.memoryOffset += 256 * sizeof(BSEventRecord_struct);
    g_BSEventModule.freeHead = g_BSEventModule.records;
    // Preserve the original full-size clear from the record-array address.
    DWI_charfill(g_BSEventModule.records, BSEventGetMemoryRequirements(), 0);
    for (int i = 0; i < 255; ++i)
        g_BSEventModule.records[i].next = &g_BSEventModule.records[i + 1];
    g_BSEventModule.records[255].next = 0;
    g_BSEventModule.pendingHead = 0;
    g_BSEventModule.pendingTail = 0;
    g_pMemBlockAllocator.Init(g_BSEventModule.memory + g_BSEventModule.memoryOffset, sizeof(BSEventMemoryView), 128, 0);
    g_BSEventModule.memoryOffset += g_pMemBlockAllocator.ComputeNeededSize(sizeof(BSEventMemoryView), 128, 0);
    g_BSEventModule.immediateHead = 0;
    g_BSEventModule.processingState = 0;
    return 0;
}

int BSEndEvent() {
    g_BSEventModule.buckets = 0;
    g_BSEventModule.records = 0;
    g_BSEventModule.freeHead = 0;
    return 0;
}
