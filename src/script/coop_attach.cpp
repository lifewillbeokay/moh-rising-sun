// AI-assisted reconstruction from GR8E69; see docs/Script.md.
// Co-op and split-screen built-ins. In this single-player build most only pop
// their arguments, and the queries return zero or -1.
#include "ScriptBuiltins.h"

void BIFunc_AttachPlayerToPropCoop2(int **stack, void *) {
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_AttachPlayerToPropCoop(int **stack, void *) {
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_AttachPlayerIDToPropCoop(int **stack, void *) {
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_DetachPlayerFromPropCoop(int **stack, void *) {
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_DetachTriggeringPlayer(int **stack, void *) {
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_IsPlayerAttached(int **stack, void *) {
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = 0;
}

void BIFunc_IsPlayerUsingHierMMG(int **stack, void *) {
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = 0;
}

void BIFunc_HowManyPlayersAttached(int **stack, void *) {
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = 0;
}
