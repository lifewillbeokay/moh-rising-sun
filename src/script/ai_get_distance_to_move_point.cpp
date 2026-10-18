// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"
#include "AITargeting.h"

void BIFunc_AIGetDistanceToMovePoint(int **stack, void *object) {
    float distance = ((ISceneNode *)object)->GetAIObject()->locomotion.GetDistanceToArbitraryPoint();
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    volatile float result = distance;
    **stack = *(int *)&result;
}
