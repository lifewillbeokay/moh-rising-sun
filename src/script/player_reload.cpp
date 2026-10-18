// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"
#include "CameraShake.h"
#include "Weapon.h"

void BIFunc_PlayerStartReloading(int **stack, void *object) {
    ((ISceneNode *)object)->AsPlayerObject()->GetCurrentWeapon()->StartReloading();
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_PlayerReloadSingle(int **stack, void *object) {
    ((ISceneNode *)object)->AsPlayerObject()->GetCurrentWeapon()->ReloadSingle();
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_PlayerClearReloading(int **stack, void *object) {
    ((ISceneNode *)object)->AsPlayerObject()->GetCurrentWeapon()->reloading = 0;
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_PlayerFinishedReloading(int **stack, void *object) {
    ((ISceneNode *)object)->AsPlayerObject()->GetCurrentWeapon()->DoneReloading();
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
