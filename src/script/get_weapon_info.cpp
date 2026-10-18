// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "SceneObjects.h"
#include "Weapon.h"

void BIFunc_GetWeaponInfo(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int result = -1;
    int target = *(*stack - (count - 1));
    EWeaponInfo info = (EWeaponInfo)*(*stack - (count - 2));
    if (target == 2) {
        CSoldierObject *soldier = (CSoldierObject *)g_pBSObject->gameObject->GetSceneNode();
        if (info == WeaponInfo2)
            result = soldier->currentWeaponSlot;
        else
            result = GetWeaponInfoHelper(soldier->weapons[soldier->currentWeaponSlot], info);
    }
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = result;
}
