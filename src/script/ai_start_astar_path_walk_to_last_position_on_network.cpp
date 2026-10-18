// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "AITargeting.h"

void SetMovePoint(EMovePointType, ISceneNode *, CVector3 &, float);

void BIFunc_AIStartAStarPathWalkToLastPositionOnNetwork(int **stack, void *object) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int started = 0;
    BSObject *target = (BSObject *)args[1];
    CAIObject *ai = ((ISceneNode *)object)->GetAIObject();
    CVector3 point;
    target->gameObject->GetSceneNode()->GetAIObject()->areaPathFinding.GetClosestPositionOnAStar(&point);
    if (!(ai->position.DistanceSquared(point) <= 1.0f)) {
        SetMovePoint(MovePointType3, (ISceneNode *)object, point, 1.2f);
        started = ai->areaPathFinding.StartAStarPathWalk(AStarDestinationUnknown);
    }
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = started;
}

void BIFunc_AtClosestPointToPlayerOffNetwork(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *player = (BSObject *)*(*stack - (count - 1));
    BSObject *soldier = (BSObject *)*(*stack - (count - 2));
    CVector3 closest;
    CVector3 position;
    soldier->gameObject->GetSceneNode()->GetAIObject()->areaPathFinding.GetClosestPositionOnAStar(&closest);
    player->gameObject->GetSceneNode()->GetPosition(position);
    float distance = position.DistanceSquared(closest);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = distance < 4.0f;
}
