// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "ScriptTimers.h"
#include "SceneNode.h"
#include "SceneObjects.h"

void BIFunc_SendEventWeaponImmediate(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *soldier = (BSObject *)*(*stack - (count - 1));
    int event = *(*stack - (count - 2));
    void *data = (void *)*(*stack - (count - 3));
    if (soldier && soldier != (BSObject *)-1 && soldier->gameObject && soldier->gameObject->GetSceneNode() &&
        soldier->gameObject->GetSceneNode()->AsSoldierObject() &&
        CurrentWeaponNode(soldier->gameObject->GetSceneNode()->AsSoldierObject()) &&
        CurrentWeaponNode(soldier->gameObject->GetSceneNode()->AsSoldierObject())->GetScriptObject())
        BSObjectTriggerEvent((BSObject *)CurrentWeaponNode(soldier->gameObject->GetSceneNode()->AsSoldierObject())->GetScriptObject(), event, data, g_pBSObject, true);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
