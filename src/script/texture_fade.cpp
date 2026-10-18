// AI-assisted reconstruction from GR8E69; see docs/Script.md.
// Player texture-fade built-ins that this build leaves as stubs.
#include "ScriptBuiltins.h"

void BIFunc_PlayerTextureFadeIn(int **stack, void *) {
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_PlayerTextureFadeOut(int **stack, void *) {
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
