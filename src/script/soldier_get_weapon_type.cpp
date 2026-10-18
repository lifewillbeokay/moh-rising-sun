// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "SceneObjects.h"
#include "Weapon.h"

void BIFunc_Soldier_GetWeaponType(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int type = -1;
    BSObject *target = (BSObject *)*(*stack - (count - 1));
    int slot = *(*stack - (count - 2));
    CWeapon *weapon = target->gameObject->GetSceneNode()->AsSoldierObject()->weapons[slot];
    if (weapon)
        type = weapon->properties->weaponType;
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = type;
}
