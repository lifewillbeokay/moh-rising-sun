// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "AITargeting.h"

void SetMovePoint(EMovePointType, ISceneNode *, CVector3 &, float);

void BIFunc_AIFindTacticalPosition(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *soldier = (BSObject *)*(*stack - (count - 1));
    float range = *(float *)(*stack - (count - 2));
    CAIObject *ai = soldier->gameObject->GetSceneNode()->GetAIObject();
    CVector3 point;
    int found = ai->tactics.FindTacticalPoint(&point, range);
    if (found)
        SetMovePoint(MovePointType3, soldier->gameObject->GetSceneNode(), point, 1.2f);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = found;
}
