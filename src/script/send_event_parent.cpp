// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "ScriptTimers.h"

void BIFunc_SendEventParent(int **stack, void *) {
    BSObjectTriggerEvent(g_pBSObject->nativeObject->scriptObject, (*stack)[-1], (void *)(*stack)[0], g_pBSObject, false);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
