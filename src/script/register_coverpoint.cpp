// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "AIBehavior.h"

void BIFunc_RegisterCoverpoint(int **stack, void *) {
    CCoverpointBehavior::RegisterCoverpoint(g_pBSObject->nativeObject);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
