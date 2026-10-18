// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "GameSettings.h"

void BIFunc_HasFilmCanister(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int level = *(*stack - (count - 1));
    int type = *(*stack - (count - 2));
    int has = 0;
    if (type == 0)
        has = (g_GameSettings.profile->records[g_GameSettings.profile->currentRecord].filmCanisters[0] >> level) & 1;
    else if (type != 1)
        has = (g_GameSettings.profile->records[g_GameSettings.profile->currentRecord].filmCanisters[1] >> level) & 1;
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = has;
}
