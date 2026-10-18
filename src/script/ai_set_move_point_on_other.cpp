// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "AITargeting.h"

void SetMovePoint(EMovePointType, ISceneNode *, CVector3 &, float);

void BIFunc_AISetMovePointOnOther(int **stack, void *) {
    CVector3 point;
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *soldier = (BSObject *)*(*stack - (count - 1));
    BSObject *marker = (BSObject *)*(*stack - (count - 2));
    float arrival = *(float *)(*stack - (count - 3));
    marker->nativeObject->GetPosition(point);
    SetMovePoint(MovePointType4, soldier->gameObject->GetSceneNode(), point, arrival);
}

void BIFunc_AIStartMovePointWalk(int **stack, void *object) {
    CAIObject *ai = ((ISceneNode *)object)->GetAIObject();
    CVector3 point;
    AIScriptDataView *data = (AIScriptDataView *)((BSObject *)((ISceneNode *)object)->GetScriptObject())->gameObject->GetScriptData();
    point.Set(data->movePoint.x, data->movePoint.y, data->movePoint.z);
    ai->locomotion.StartWalkToArbitraryPoint(point);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
