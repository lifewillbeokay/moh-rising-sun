// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptEvents.h"

int BSEventTriggerEvent(BSObject *object, BSMachineThread_struct *thread, BSCode_EventHandlerEntry_struct *handler, unsigned short state, void *context, BSObject *sender, unsigned short event) {
    int owned = 0;
    if (g_pMemBlockAllocator.DoWeOwnThisMemory(context)) owned = 1;
    if (owned) BSEventIncrimentReferenceCount(context);
    if (object->queueIdentity == ~0U) {
        if (owned) BSEventDecrimentReferenceCount(context);
        return 14;
    }
    BSEventRecord_struct *record = BSEventGetFreeEventRecord();
    record->thread = thread;
    record->next = 0;
    record->declaredState = state;
    record->eventNumber = event;
    record->context = context;
    record->sender = sender;
    record->object = object;
    record->handler = handler;
    record->objectIdentity = object->queueIdentity;
    record->ownsEventMemory = owned;
    int bucket = 1;
    if (handler) bucket = handler->queueBucket;
    if (g_BSEventModule.buckets[bucket].tail)
        g_BSEventModule.buckets[bucket].tail->next = record;
    g_BSEventModule.buckets[bucket].tail = record;
    if (!g_BSEventModule.buckets[bucket].head)
        g_BSEventModule.buckets[bucket].head = record;
    return 0;
}

int BSEventQueueImmediateEvent(BSObject *object, BSMachineThread_struct *thread, BSCode_EventHandlerEntry_struct *handler, unsigned short state, void *context, BSObject *sender, unsigned short event) {
    int owned = 0;
    if (g_pMemBlockAllocator.DoWeOwnThisMemory(context)) owned = 1;
    if (owned) BSEventIncrimentReferenceCount(context);
    if (object->queueIdentity == ~0U) {
        if (owned) BSEventDecrimentReferenceCount(context);
        return 14;
    }
    BSEventRecord_struct *record = BSEventGetFreeEventRecord();
    record->thread = thread;
    record->next = 0;
    record->declaredState = state;
    record->handler = handler;
    record->eventNumber = event;
    record->context = context;
    record->sender = sender;
    record->object = object;
    record->objectIdentity = object->queueIdentity;
    record->ownsEventMemory = owned;
    if (g_BSEventModule.immediateHead) {
        g_BSEventModule.immediateTail->next = record;
        g_BSEventModule.immediateTail = record;
    } else {
        g_BSEventModule.immediateHead = record;
        g_BSEventModule.immediateTail = record;
    }
    return 0;
}
