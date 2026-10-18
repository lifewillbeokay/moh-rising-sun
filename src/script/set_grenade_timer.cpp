// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "Bullet.h"

void BIFunc_SetGrenadeTimer(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    CBullet *grenade = (CBullet *)*(*stack - (count - 1));
    grenade->field_b0 = *(*stack - (count - 2));
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
