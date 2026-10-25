#ifndef GAME_SCRIPT_TIMERS_H
#define GAME_SCRIPT_TIMERS_H
#include "ScriptRuntime.h"

// GR8E69 runtime storage, with a verified 24-byte event stride and eight-byte
// bucket stride. Member and bucket names are descriptive, not original names.
struct BSTimerEvent_struct {
    int time;
    unsigned short eventNumber;
    unsigned char unknown06[2];
    BSObject *object;
    void *context;
    int ownsEventMemory;
    BSTimerEvent_struct *next;
};
struct BSTimerBucketView {
    BSTimerEvent_struct *head;
    BSTimerEvent_struct *tail;
};
extern BSTimerBucketView *g_pTimerHashTable;
extern BSTimerEvent_struct *g_pTimerEventArray;
extern BSTimerEvent_struct *g_pFreeTimerEventHead;
extern int g_LastHashIndex;
extern float g_CurrentTime;
extern unsigned char *g_pBSTimerMemory;
extern int g_pBSTimerMemoryOffset;
extern void BSEventDecrimentReferenceCount(void *);
extern void *BSUtilGetMemory(int, int);
extern int BSObjectTriggerEvent(BSObject *, unsigned short, void *, BSObject *, bool);
extern int BSTimerGetMemoryRequirements();
extern BSTimerEvent_struct *BSGetFreeTimerEvent();
extern void BSTimerRemoveTimerEvent(BSTimerEvent_struct *, BSTimerEvent_struct **);
extern void BSTimerRemoveDuplicateTimerInstances(BSObject *, unsigned short, int);
extern int BSTimerRemoveLatterTimerInstances(BSObject *, unsigned short, int);
extern void BSTimerRemoveTimerInstances(BSObject *);
#include "ScriptPool.h"
extern BSUtilObjectInstanceMemoryAllocator g_pMemBlockAllocator;
extern void BSEventIncrimentReferenceCount(void *);
// The enum tag is recovered; enumerator names describe observed cases.
enum ETimerReplaceMethod {
    TIMER_REPLACE_DUPLICATES = 0,
    TIMER_KEEP_EARLIER = 1,
    TIMER_APPEND = 2
};
void BSRegisterTimerEvent(int, unsigned short, BSObject *, void *, ETimerReplaceMethod);
void BSCancelTimerEvent(unsigned short, BSObject *);
#endif
