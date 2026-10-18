// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"

void BIFunc_GetMyInstanceData(int **stack, void *) {
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = (int)g_pBSObject->gameObject->GetScriptData();
}
