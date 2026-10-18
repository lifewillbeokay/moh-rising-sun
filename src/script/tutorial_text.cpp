// AI-assisted reconstruction from GR8E69; see docs/Script.md.
// Prompt, tutorial-text and SED-event built-ins that this build leaves as stubs.
#include "ScriptBuiltins.h"

void BIFunc_DisplayBIGPromptText(int **stack, void *) {
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_DisplayTutorialText(int **stack, void *) {
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_KillTutorialText(int **stack, void *) {
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_ToggleTutorialIcon(int **stack, void *) {
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_SEDEvent(int **stack, void *) {
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
