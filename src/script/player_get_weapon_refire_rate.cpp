// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"
#include "CameraShake.h"
#include "Weapon.h"

void BIFunc_PlayerGetWeaponRefireRate(int **stack, void *object) {
    CWeapon *weapon = ((ISceneNode *)object)->AsPlayerObject()->GetCurrentWeapon();
    int rate = 0;
    if (weapon)
        rate = weapon->properties->refireRate;
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = rate;
}
