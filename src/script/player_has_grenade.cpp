// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "CameraShake.h"

void BIFunc_PlayerHasGrenade(int **stack, void *object) {
    bool has = ((CPlayerObject *)object)->GetNumGrenades() != 0;
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = has;
}
