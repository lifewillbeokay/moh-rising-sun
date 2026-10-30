// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptEvents.h"

int BSEventProcessImmediateEvents() {
    if (!g_BSEventModule.processingState) {
        BSEventRecord_struct *record = g_BSEventModule.immediateHead;
        BSEventRecord_struct *last = 0;
        g_BSEventModule.processingState = 2;
        while (record) {
            BSEventProcessSingleEvent(record);
            if (record->ownsEventMemory) BSEventDecrimentReferenceCount(record->context);
            last = record;
            record = record->next;
        }
        if (g_BSEventModule.immediateHead) {
            last->next = g_BSEventModule.freeHead;
            g_BSEventModule.freeHead = g_BSEventModule.immediateHead;
        }
        g_BSEventModule.immediateHead = 0;
        g_BSEventModule.immediateTail = 0;
        g_BSEventModule.processingState = 0;
    }
    return 0;
}

int BSEventProcessEvents(int limit) {
    g_BSEventModule.processingState = 1;
    int found = 0;
    BSEventRecord_struct *tail = 0;
    for (int i = 0; i < 16; ++i) {
        if (g_BSEventModule.buckets[i].head) {
            if (found) {
                tail->next = g_BSEventModule.buckets[i].head;
                g_BSEventModule.pendingTail = g_BSEventModule.buckets[i].tail;
                tail = g_BSEventModule.buckets[i].tail;
            } else {
                if (g_BSEventModule.pendingTail)
                    g_BSEventModule.pendingTail->next = g_BSEventModule.buckets[i].head;
                else
                    g_BSEventModule.pendingHead = g_BSEventModule.buckets[i].head;
                // The original leaves pendingTail unchanged for this first bucket.
                found = 1;
                tail = g_BSEventModule.buckets[i].tail;
            }
        }
        g_BSEventModule.buckets[i].head = 0;
        g_BSEventModule.buckets[i].tail = 0;
    }
    BSEventRecord_struct *record = g_BSEventModule.pendingHead;
    int count = 0;
    BSEventRecord_struct *last = 0;
    while (record && count < limit) {
        g_BSEventModule.current = record;
        BSEventProcessSingleEvent(record);
        if (record->ownsEventMemory) BSEventDecrimentReferenceCount(record->context);
        last = record;
        ++count;
        record = record->next;
        g_BSEventModule.processingState = 0;
        BSEventProcessImmediateEvents();
        g_BSEventModule.processingState = 1;
    }
    if (last) {
        last->next = g_BSEventModule.freeHead;
        g_BSEventModule.freeHead = g_BSEventModule.pendingHead;
    }
    g_BSEventModule.pendingHead = record;
    if (!record) g_BSEventModule.pendingTail = 0;
    g_BSEventModule.processingState = 0;
    return 0;
}
