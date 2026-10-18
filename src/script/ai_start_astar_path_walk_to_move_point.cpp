// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"
#include "AIBehavior.h"

void BIFunc_AIStartAStarPathWalkToMovePoint(int **stack, void *object) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    EAStarDestinationInfo info = (EAStarDestinationInfo)args[1];
    int result = ((ISceneNode *)object)->GetAIObject()->areaPathFinding.StartAStarPathWalk(info);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = result;
}

void BIFunc_AIStopAStarPathWalk(int **stack, void *object) {
    ((ISceneNode *)object)->GetAIObject()->areaPathFinding.StopAStarPathWalk();
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_AIStartCoverPointWalk(int **stack, void *object) {
    int result = ((CCoverpointBehavior *)((ISceneNode *)object)->GetAIObject()->FindMatchingBehavior(BehaviorTypeCoverpoint))->StartCoverPointWalk();
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = result;
}

void BIFunc_AIStopCoverPointWalk(int **stack, void *object) {
    ((CCoverpointBehavior *)((ISceneNode *)object)->GetAIObject()->FindMatchingBehavior(BehaviorTypeCoverpoint))->StopCoverPointWalk();
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
