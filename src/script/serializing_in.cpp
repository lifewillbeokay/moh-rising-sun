// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "GameSettings.h"

void BIFunc_SerializingIn(int **stack, void *) {
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = g_GameSettings.SerializingIn();
}

void BIFunc_SerializingOut(int **stack, void *) {
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = g_GameSettings.SerializingOut();
}
