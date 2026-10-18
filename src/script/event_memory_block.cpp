// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"

void *BSEventGetEventMemoryBlock(unsigned int);

void BIFunc_GetEventSystemMemBlock(int **stack, void *) {
    void *block = BSEventGetEventMemoryBlock(32);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = (int)block;
}
