// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "GameSettings.h"

void BIFunc_AddFilmCanister(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int type = args[2];
    if (type == 0) {
        unsigned int level = g_GameSettings.profile->level;
        if (level <= 10)
            g_GameSettings.profile->records[g_GameSettings.profile->currentRecord].filmCanisters[0] |= 1 << level;
    } else if (type != 1) {
        unsigned int level = g_GameSettings.profile->level;
        if (level <= 10)
            g_GameSettings.profile->records[g_GameSettings.profile->currentRecord].filmCanisters[1] |= 1 << level;
    }
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
