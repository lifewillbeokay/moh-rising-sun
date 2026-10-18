// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"
#include "CameraShake.h"
#include "Weapon.h"

void BIFunc_PlayerIsMyWeaponAProjectile(int **stack, void *object) {
    CPlayerObject *player = (CPlayerObject *)object;
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    bool projectile = player->GetCurrentWeapon()->properties->projectile;
    **stack = projectile;
}

void BIFunc_SetPlayerWielding(int **stack, void *object) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    ((CPlayerObject *)object)->flags.wielding = args[1];
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_PlayerFireMyWeapon(int **stack, void *object) {
    ((ISceneNode *)object)->AsPlayerObject()->FireMyWeapon(
        reinterpret_cast<float *>(*stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount)[1]);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
