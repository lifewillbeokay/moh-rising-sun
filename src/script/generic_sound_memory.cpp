// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"

// Original 80-byte storage; this declaration does not allocate it.
extern unsigned char g_pGenericSoundMemory[80];

void BIFunc_GetGenericSoundStructureMemory(int **stack, void *) {
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = (int)g_pGenericSoundMemory;
}
