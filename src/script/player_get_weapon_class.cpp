// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"
#include "CameraShake.h"
#include "Weapon.h"

void BIFunc_PlayerGetWeaponClass(int **stack, void *object) {
    int weaponClass = 0;
    CPlayerObject *player = ((ISceneNode *)object)->AsPlayerObject();
    if (player->GetCurrentWeapon())
        weaponClass = player->GetCurrentWeapon()->weaponClass;
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = weaponClass;
}
