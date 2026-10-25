#include "ScriptPool.h"
#include "ScriptRuntime.h"
#include "ScriptStartup.h"
#include "MemStack.h"
extern unsigned char *g_pScriptMemory;
extern int g_memOffset;
extern EScriptSystemState g_script_system_state;
extern MemStack *g_pScriptMemStack;
extern BSUtilObjectInstanceMemoryAllocator g_pMemAllocators[4];
void *BSUtilGetMemory(int size, int alignment) {
    int offset = g_memOffset;
    if (alignment) {
        offset += alignment - (offset & (alignment - 1));
    }
    g_memOffset = offset + size;
    return g_pScriptMemory + offset;
}
void *BSUtilGetObjectInstanceMemory(BSClass_struct *scriptClass) {
    int size = scriptClass->instanceMemorySize;
    if (g_script_system_state == SCRIPT_SYSTEM_STARTUP) {
        return g_pScriptMemStack->Alloc(size, 4);
    }
    for (int i = 0; i < 4; ++i) {
        if (g_pMemAllocators[i].PayloadSize() >= size) {
            void *memory = g_pMemAllocators[i].GetFreeElement(false);
            if (memory) return memory;
        }
    }
    return 0;
}
void BSUtilFreeObjectInstanceMemory(void *memory) {
    for (int i = 0; i < 4; ++i) {
        if (g_pMemAllocators[i].DoWeOwnThisMemory(memory)) {
            g_pMemAllocators[i].FreeElement(memory);
            return;
        }
    }
}
