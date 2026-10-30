#ifndef GAME_SCRIPT_EVENTS_H
#define GAME_SCRIPT_EVENTS_H
#include "ScriptRuntime.h"
#include "ScriptPool.h"

// Scoped runtime storage. Field names are descriptive; unknown bytes remain opaque.
struct BSEventRecord_struct {
    BSObject *object;
    BSMachineThread_struct *thread;
    BSEventRecord_struct *next;
    BSCode_EventHandlerEntry_struct *handler;
    unsigned short eventNumber;
    unsigned char unknown12[2];
    void *context;
    BSObject *sender;
    unsigned int objectIdentity;
    int ownsEventMemory;
    unsigned short declaredState;
    unsigned char unknown26[2];
};
struct BSEventBucketView {
    BSEventRecord_struct *head;
    BSEventRecord_struct *tail;
};
struct BSEventModuleView {
    BSEventBucketView *buckets;
    BSEventRecord_struct *records;
    BSEventRecord_struct *freeHead;
    BSEventRecord_struct *pendingHead;
    BSEventRecord_struct *pendingTail;
    BSEventRecord_struct *current;
    BSEventRecord_struct *immediateHead;
    BSEventRecord_struct *immediateTail;
    int processingState;
    unsigned char *memory;
    int memoryOffset;
};
struct BSEventMemoryView {
    unsigned char payload[32];
    int referenceCount;
};
typedef char BSEventRecordSizeCheck[sizeof(BSEventRecord_struct) == 40 ? 1 : -1];
typedef char BSEventModuleSizeCheck[sizeof(BSEventModuleView) == 44 ? 1 : -1];
typedef char BSEventMemorySizeCheck[sizeof(BSEventMemoryView) == 36 ? 1 : -1];
extern BSEventModuleView g_BSEventModule;
extern BSUtilObjectInstanceMemoryAllocator g_pMemBlockAllocator;
extern void *BSUtilGetMemory(int, int);
extern "C" void DWI_charfill(void *, int, int);
int BSEventGetMemoryRequirements();
extern BSEventRecord_struct *BSEventGetFreeEventRecord();
extern void BSEventIncrimentReferenceCount(void *);
extern void BSEventDecrimentReferenceCount(void *);
extern int BSEventProcessSingleEvent(BSEventRecord_struct *);
extern int BSEventProcessImmediateEvents();
extern int BSEventFindAnyEventHandler(unsigned short, BSClass_struct *);
extern unsigned short BSEventFindDeclaredState(unsigned short, BSMachineThread_struct *, BSCode_EventHandlerEntry_struct **);
extern int BSEventTriggerEvent(BSObject *, BSMachineThread_struct *, BSCode_EventHandlerEntry_struct *, unsigned short, void *, BSObject *, unsigned short);
extern int BSEventQueueImmediateEvent(BSObject *, BSMachineThread_struct *, BSCode_EventHandlerEntry_struct *, unsigned short, void *, BSObject *, unsigned short);
extern int BSEventIsEventMemoryBlock(void *);
extern void BSEventCheckMemBlockReferenceCount(void *);
#endif
