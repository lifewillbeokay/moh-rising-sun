// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptStartup.h"
#include "MemStack.h"
extern MemStack *g_pScriptMemStack;
void BSUtilInitStartupAllocator(int size) {
    delete g_pScriptMemStack;
    g_pScriptMemStack = new ("source/ai_script/bsutil.cpp:906") MemStack(size);
}
