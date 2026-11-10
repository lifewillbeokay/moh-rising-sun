// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "GameSettings.h"

void BIFunc_HasAnyFilmCanister(int **stack, void *) {
    int level = g_GameSettings.profile->saveData.level;
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    int second = (g_GameSettings.profile->profiles[g_GameSettings.profile->profileIndex[0]].filmCanisters[1] >> level) & 1;
    int first = (g_GameSettings.profile->profiles[g_GameSettings.profile->profileIndex[0]].filmCanisters[0] >> level) & 1;
    **stack = (second != 0) | (first != 0);
}
