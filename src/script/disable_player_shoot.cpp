// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"
#include "CameraShake.h"
#include "Weapon.h"

void BIFunc_DisablePlayerShoot(int **stack, void *object) {
    CPlayerObject *player = ((ISceneNode *)object)->AsPlayerObject();
    if (player->GetCurrentWeapon())
        player->GetCurrentWeapon()->Shoot((EWeaponShootType)1, 0.0f, -1.0f);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
