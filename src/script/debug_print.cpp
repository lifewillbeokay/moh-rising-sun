// AI-assisted reconstruction from GR8E69; see docs/Script.md.
// Debug-output built-ins that this build leaves as stubs.
#include "ScriptBuiltins.h"

void BIFunc_DebugTTYPrint(int **stack, void *) {
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_AILog(int **stack, void *) {
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_ScriptError(int **stack, void *) {
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
