// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "Weapon.h"

// Observed script-data prefix, not a complete allocation or historical type.
struct GrenadeScriptDataView {
    unsigned char unknown00[0x84];
    int grenadeSlot;
};

CWeapon *GetSoldierWeaponInSlot(int);

void BIFunc_SetGrenadeSlot(int **stack, void *object) {
    int slot = -1;
    for (int i = 0; i < 4; i++) {
        CWeapon *weapon = GetSoldierWeaponInSlot(i);
        if (weapon && weapon->weaponClass == 7) {
            slot = i;
            break;
        }
    }
    GrenadeScriptDataView *data = (GrenadeScriptDataView *)
        ((BSObject *)((ISceneNode *)object)->GetScriptObject())->gameObject->GetScriptData();
    data->grenadeSlot = slot;
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
