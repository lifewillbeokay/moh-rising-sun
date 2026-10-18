// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "Bullet.h"

void BIFunc_SetDetonationTime(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    CThrownBullet *grenade = (CThrownBullet *)*(*stack - (count - 1));
    float seconds = *(float *)(*stack - (count - 2));
    grenade->SetDetonationTime(seconds * 60.0f);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
