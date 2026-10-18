// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include <stdio.h>
#include "CameraShake.h"

void BIFunc_PlayWeaponAnimation(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    char *animation = (char *)*(*stack - (count - 1));
    int mode = *(*stack - (count - 2));
    CPlayerWeaponObject *weapon = (CPlayerWeaponObject *)*(*stack - (count - 3));
    CPlayerWeaponObject::Partition partition = (CPlayerWeaponObject::Partition)(*(*stack - (count - 4)) == 4);
    char state[256];
    sprintf(state, "PlayerWeapon\\%s\\%s:%s", weapon->name, weapon->name, animation);
    weapon->SetAnimationState(state, mode, partition);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
