// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "AITargeting.h"

void BIFunc_Soldier_AIGetDistanceToMovePoint(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *soldier = (BSObject *)args[1];
    float distance = soldier->gameObject->GetSceneNode()->GetAIDoodad()->filter->object->locomotion.GetDistanceToArbitraryPoint();
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    volatile float result = distance;
    **stack = *(int *)&result;
}
