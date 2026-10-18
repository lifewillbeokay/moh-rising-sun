// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "SceneObjects.h"
#include "Weapon.h"

void BIFunc_GetCurrentNPCWeaponClass(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *target = (BSObject *)args[1];
    ISceneNode *node = 0;
    if (target && target->gameObject && target->gameObject->GetSceneNode())
        node = target->gameObject->GetSceneNode();
    CSoldierObject *soldier = node->AsSoldierObject();
    int type = -1;
    CWeapon *weapon = soldier->weapons[soldier->currentWeaponSlot];
    if (weapon)
        type = weapon->properties->weaponType;
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = type;
}
