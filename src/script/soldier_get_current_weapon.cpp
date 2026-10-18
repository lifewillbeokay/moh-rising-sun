// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "SceneObjects.h"
#include "Weapon.h"

void BIFunc_Soldier_GetCurrentWeapon(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int type = -1;
    BSObject *target = (BSObject *)args[1];
    if (target) {
        CSoldierObject *soldier = target->gameObject->GetSceneNode()->AsSoldierObject();
        CWeapon *weapon = soldier->weapons[soldier->currentWeaponSlot];
        if (weapon)
            type = weapon->properties->weaponType;
    }
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = type;
}
