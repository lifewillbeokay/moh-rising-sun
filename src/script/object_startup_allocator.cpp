// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptStartup.h"
#include "MemStack.h"
extern MemStack *g_pBSObjectMemStack;
extern unsigned int g_numBSObjectsOnMemStack;
void BSInitBSObjectStartupAllocator(int size) {
    delete g_pBSObjectMemStack;
    g_pBSObjectMemStack = new ("source/ai_script/bsobject.cpp:578") MemStack(size);
    g_numBSObjectsOnMemStack = (unsigned int)size / 48;
}
