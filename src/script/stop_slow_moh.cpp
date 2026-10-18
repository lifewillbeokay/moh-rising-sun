// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"

void SlowMOH_Stop();

void BIFunc_StopSlowMOH(int **stack, void *) {
    SlowMOH_Stop();
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
