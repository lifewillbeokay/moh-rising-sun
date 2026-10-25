#include "ScriptRuntime.h"
#include "ScriptStartup.h"
BSObject *GetFreeBSObject();
int BSFileGetClass(const char *, BSClass_struct **);
void *BSUtilGetObjectInstanceMemory(BSClass_struct *);
int BSCreateThread(BSObject *, BSCode_struct *, BSMachineThread_struct *, void *);
void AddBSObjectToList(BSObject *);
extern unsigned int g_uniqueIDCount;
int CreateBSObject(const char *name, BSObject **object, BSGO_Basic *gameObject,
                   TriggerObject *nativeObject, void **context) {
    *object = GetFreeBSObject();
    if (context) *context = *object;
    (*object)->scriptClass = 0;
    BSFileGetClass(name, &(*object)->scriptClass);
    if (!(*object)->scriptClass) return 13;
    (*object)->threads = (BSMachineThread_struct *)BSUtilGetObjectInstanceMemory((*object)->scriptClass);
    (*object)->queueIdentity = g_uniqueIDCount++;
    (*object)->nativeObject = nativeObject;
    (*object)->gameObject.Assign(gameObject);
    (*object)->field_28 = 0;
    unsigned int offset = 0;
    int *values = (int *)((*object)->threads + (*object)->scriptClass->levelCount);
    for (unsigned int i = 0; i < (*object)->scriptClass->levelCount; ++i) {
        BSCode_struct *level = &(*object)->scriptClass->levels[i];
        int result = BSCreateThread(*object, level, &(*object)->threads[i], values + offset);
        if (result) return result;
        offset += level->contextWordCount;
    }
    AddBSObjectToList(*object);
    return 0;
}
