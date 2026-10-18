// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "SceneObjects.h"

void BIFunc_Soldier_AIFireAtCurrentTarget(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *soldier = (BSObject *)*(*stack - (count - 1));
    float arg0 = *(float *)&*(*stack - (count - 2));
    EFireMethod arg1 = (EFireMethod)*(*stack - (count - 3));
    soldier->gameObject->GetSceneNode()->AsSoldierObject()->FireCurrentWeapon(arg0, arg1);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
