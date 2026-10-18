// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"

void DetachFromSoldier(int, bool, ISceneNode *);

void BIFunc_Soldier_DetachFromSoldier(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *soldier = (BSObject *)*(*stack - (count - 1));
    int slot = *(*stack - (count - 2));
    bool drop = *(bool *)(*stack - (count - 3));
    DetachFromSoldier(slot, drop, soldier->gameObject->GetSceneNode());
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
