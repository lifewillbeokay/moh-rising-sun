#include "ScriptTimers.h"
void BSUnregisterTimerEvents(BSObject *);
int BSDestroyThread(BSMachineThread_struct *);
void BSUtilFreeObjectInstanceMemory(void *);
void RemoveBSObjectFromList(BSObject *);
void FreeBSObject(BSObject *);
int DestroyBSObject(BSObject *object) {
    BSObjectTriggerEvent(object, 334, 0, 0, true);
    object->queueIdentity = ~0U;
    for (unsigned int i = 0; i < object->scriptClass->levelCount; ++i)
        BSDestroyThread(&object->threads[i]);
    BSUnregisterTimerEvents(object);
    BSUtilFreeObjectInstanceMemory(object->threads);
    RemoveBSObjectFromList(object);
    FreeBSObject(object);
    return 0;
}
