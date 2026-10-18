// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"
#include "AITargeting.h"

void BIFunc_AILoopSplinePathWalk(int **stack, void *object) {
    ((ISceneNode *)object)->GetAIObject()->splinePath.LoopSplinePathWalk();
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
