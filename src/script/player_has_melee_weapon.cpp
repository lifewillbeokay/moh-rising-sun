// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"
#include "CameraShake.h"

void BIFunc_PlayerHasMeleeWeapon(int **stack, void *object) {
    int melee = 0;
    CPlayerObject *player = ((ISceneNode *)object)->AsPlayerObject();
    if (player->GetCurrentWeapon()) {
        melee = 1;
        player->GetCurrentWeapon();
    }
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = melee;
}

void BIFunc_PlayerCanSwitchWeapon(int **stack, void *object) {
    int result = ((ISceneNode *)object)->AsPlayerObject()->CanPlayerSwitchWeapon();
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = result;
}
