#include "ScriptRuntime.h"
#include "ScriptPool.h"
#include "ScriptStartup.h"
#include "MemStack.h"
extern BSUtilObjectInstanceMemoryAllocator g_pBSObjectAllocator;
extern MemStack *g_pBSObjectMemStack;
extern unsigned int g_numBSObjectsOnMemStack;
inline void *operator new(unsigned int, void *p) { return p; }
BSObject *BSGetObjectByIndex(short index) {
    if (index == -1) return 0;
    if (index < (int)g_numBSObjectsOnMemStack)
        return (BSObject *)g_pBSObjectMemStack->base + index;
    return (BSObject *)g_pBSObjectAllocator.GetObjectByIndex(index - g_numBSObjectsOnMemStack);
}
short BSGetIndexByObject(BSObject *object) {
    int index;
    if (g_pBSObjectAllocator.DoWeOwnThisMemory(object))
        index = g_numBSObjectsOnMemStack + g_pBSObjectAllocator.GetIndexOfObject(object);
    else
        index = object - (BSObject *)g_pBSObjectMemStack->base;
    return index;
}
BSObject *GetFreeBSObject() {
    BSObject *object;
    if (BSUtilGetScriptingSystemState() == SCRIPT_SYSTEM_STARTUP)
        object = (BSObject *)g_pBSObjectMemStack->Alloc(sizeof(BSObject), 4);
    else
        object = (BSObject *)g_pBSObjectAllocator.GetFreeElement(true);
    if (object) new (object) BSObject;
    return object;
}
void FreeBSObject(BSObject *object) {
    object->~BSObject();
    if (g_pBSObjectAllocator.DoWeOwnThisMemory(object))
        g_pBSObjectAllocator.FreeElement(object);
}
