// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "GameSettings.h"

void BIFunc_HasReplayItem(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int result = 0;
    int item = args[1];
    if ((item == 7 ? g_GameSettings.HasMedal(g_GameSettings.profile->level)
                   : CurrentRecordHasReplayItem(item)) ||
        g_cheats.allReplayItems)
        result = 1;
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = result;
}
