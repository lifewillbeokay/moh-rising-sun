// AI-assisted reconstruction from GR8E69; see docs/Script.md.
// Player action built-ins that this build leaves as stubs.
#include "ScriptBuiltins.h"

void BIFunc_PlayerGeneratedTrigger(int **stack, void *) {
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = 0;
}

void BIFunc_SetFullscreenMode(int **stack, void *) {
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_PlayerToggleDuckForShoulderClimb(int **stack, void *) {
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_PlayerJumpOverShoulderClimb(int **stack, void *) {
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_PlayerInsideCrouchAction(int **stack, void *) {
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_PlayerOutsideCrouchAction(int **stack, void *) {
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_ToggleCoopMechanicIcon(int **stack, void *) {
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
