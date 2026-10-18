// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"
#include "SceneObjects.h"
#include "Weapon.h"

void BIFunc_SetWeaponDraw(int **stack, void *object) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    bool draw = reinterpret_cast<bool *>(args)[1];
    CSoldierObject *soldier = ((ISceneNode *)object)->AsSoldierObject();
    ISceneNode *weapon = (ISceneNode *)soldier->weapons[soldier->currentWeaponSlot];
    if (weapon)
        weapon->SetDrawEnabled(draw);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
