// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"
#include "SceneObjects.h"
#include "Weapon.h"

void BIFunc_GetCurrentWeaponClass(int **stack, void *object) {
    CSoldierObject *soldier = ((ISceneNode *)object)->AsSoldierObject();
    int type = -1;
    CWeapon *weapon = soldier->weapons[soldier->currentWeaponSlot];
    if (weapon)
        type = weapon->properties->weaponType;
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = type;
}
