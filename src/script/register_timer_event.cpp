// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "ScriptTimers.h"

void BIFunc_RegisterTimerEvent(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSRegisterTimerEvent(*(*stack - (count - 1)), *(*stack - (count - 2)), g_pBSObject,
                         (void *)*(*stack - (count - 3)), TIMER_REPLACE_DUPLICATES);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
