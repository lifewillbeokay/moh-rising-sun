// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "GameSettings.h"

void BIFunc_HasAnyFilmCanister(int **stack, void *) {
    int level = g_GameSettings.profile->level;
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    int second = (g_GameSettings.profile->records[g_GameSettings.profile->currentRecord].filmCanisters[1] >> level) & 1;
    int first = (g_GameSettings.profile->records[g_GameSettings.profile->currentRecord].filmCanisters[0] >> level) & 1;
    **stack = (second != 0) | (first != 0);
}
