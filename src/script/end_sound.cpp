// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SoundManager.h"

void BIFunc_EndSound(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int sound = args[1];
    if (sound > 0) {
        SoundManagerView *manager = &g_SoundMan;
        manager->schedule.RemoveSound(sound);
    }
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
