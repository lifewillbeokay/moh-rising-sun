#include "ScriptPool.h"
#include "MemStack.h"
struct BSObject;
extern BSUtilObjectInstanceMemoryAllocator g_pBSObjectAllocator;
extern BSObject *g_pBSObjectListStart, *g_pBSObjectListEnd;
extern MemStack *g_pBSObjectMemStack;
extern unsigned int g_numBSObjectsOnMemStack;
extern void *BSUtilGetMemory(int, int);
extern void DestroyBSObject(BSObject *);
int BSObjectGetMemoryRequirements() {
    return g_pBSObjectAllocator.ComputeNeededSize(48, 228, 0);
}
int BSInitBSObject() {
    void *memory = BSUtilGetMemory(BSObjectGetMemoryRequirements(), 0);
    g_pBSObjectAllocator.Init(memory, 48, 228, 0);
    return 0;
}
void BSFreeAllBSObject() {
    while (g_pBSObjectListStart) DestroyBSObject(g_pBSObjectListStart);
    g_pBSObjectListStart = 0;
    g_pBSObjectListEnd = 0;
    delete g_pBSObjectMemStack;
    g_pBSObjectMemStack = 0;
    g_numBSObjectsOnMemStack = 0;
}
