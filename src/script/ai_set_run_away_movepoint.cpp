// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "AITargeting.h"

void SetMovePoint(EMovePointType, ISceneNode *, CVector3 &, float);

void BIFunc_AISetRunAwayMovepoint(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *soldier = (BSObject *)*(*stack - (count - 1));
    ISceneNode *from = (ISceneNode *)*(*stack - (count - 2));
    float distance = *(float *)(*stack - (count - 3));
    bool flag = *(bool *)(*stack - (count - 4));
    CVector3 point;
    int found = soldier->gameObject->GetSceneNode()->GetAIObject()->FindRunAwayPoint(from, distance, &point, flag);
    if (found)
        SetMovePoint(MovePointType3, soldier->gameObject->GetSceneNode(), point, 1.2f);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = found;
}
