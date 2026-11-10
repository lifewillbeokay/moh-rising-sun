// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "GameSettings.h"

void BIFunc_EndLevel(int **stack, void *) {
    g_GameSettings.profile->saveData.levelWon = *(*stack - (g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount - 1));
    g_GameFlow.levelEnding = 1;
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
