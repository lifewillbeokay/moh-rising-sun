// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SoundManager.h"

void BIFunc_ActivateSoundBuss(int **stack, void *) {
    SoundManagerView *manager = &g_SoundMan;
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    manager->schedule.ActivateBuss(args[1]);
}
