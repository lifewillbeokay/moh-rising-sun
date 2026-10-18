// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "AITargeting.h"

int ChooseTargetByID(CAIObject *, BSObject *);

void BIFunc_AIChooseTargetByID(int **stack, void *object) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *target = (BSObject *)args[1];
    int result = ChooseTargetByID(((ISceneNode *)object)->GetAIObject(), target);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = result;
}

void BIFunc_Soldier_AIChooseTargetByID(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *soldier = (BSObject *)*(*stack - (count - 1));
    BSObject *target = (BSObject *)*(*stack - (count - 2));
    int result = ChooseTargetByID(soldier->gameObject->GetSceneNode()->GetAIObject(), target);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = result;
}
