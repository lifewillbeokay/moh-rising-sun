// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "AIMovement.h"
#include "MathFun.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"

void BIFunc_AIGetPanAngleToMovePoint(int **stack, void *object) {
    ISceneNode *node = (ISceneNode *)object;
    AIScriptDataView *data = (AIScriptDataView *)((BSObject *)node->GetScriptObject())->gameObject->GetScriptData();
    CVector3 target(data->movePoint.x, data->movePoint.y, data->movePoint.z);
    CVector3 position;
    node->GetPosition(position);
    CVector3 direction;
    node->GetRightward(direction);
    float angle = MathFunGetPanAngleDiffNoRoll(&position, &target, &direction);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    volatile float result = angle;
    **stack = *(int *)&result;
}
