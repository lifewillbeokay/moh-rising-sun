// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "AITargeting.h"

void BIFunc_AIStartAStarPathWalk(int **stack, void *object) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *target = (BSObject *)*(*stack - (count - 1));
    EAStarDestinationInfo info = (EAStarDestinationInfo)*(*stack - (count - 2));
    int result = ((ISceneNode *)object)->GetAIObject()->areaPathFinding.StartAStarPathWalk(target, info);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = result;
}
