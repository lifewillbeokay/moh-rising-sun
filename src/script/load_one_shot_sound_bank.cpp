// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "GameSettings.h"

extern int g_CurrentLevel;
extern int g_CurrentSoundBank;
extern int g_CurrentMission;
void LoadOneShotSound(int, int, int);

void BIFunc_LoadOneShotSoundBank(int **stack, void *) {
    int mission = g_GameSettings.profile->mission;
    int level = g_GameSettings.profile->level;
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int bank = args[1];
    g_CurrentMission = mission;
    g_CurrentLevel = level;
    g_CurrentSoundBank = bank;
    LoadOneShotSound(bank, mission, level);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
