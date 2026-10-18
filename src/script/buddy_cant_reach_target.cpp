// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "AIBehavior.h"

void BIFunc_BuddyCantReachTarget(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *soldier = (BSObject *)args[1];
    ((CBuddyFollowBehavior *)soldier->gameObject->GetSceneNode()->GetAIObject()->FindMatchingBehavior(BehaviorTypeBuddyFollow))->CantReachTarget();
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_BuddyFinishedMovingAway(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *soldier = (BSObject *)args[1];
    ((CBuddyFollowBehavior *)soldier->gameObject->GetSceneNode()->GetAIObject()->FindMatchingBehavior(BehaviorTypeBuddyFollow))->FinishedMovingAway();
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_SetDistances(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *soldier = (BSObject *)*(*stack - (count - 1));
    float arg0 = *(float *)&*(*stack - (count - 2));
    float arg1 = *(float *)&*(*stack - (count - 3));
    float arg2 = *(float *)&*(*stack - (count - 4));
    float arg3 = *(float *)&*(*stack - (count - 5));
    ((CBuddyFollowBehavior *)soldier->gameObject->GetSceneNode()->GetAIObject()->FindMatchingBehavior(BehaviorTypeBuddyFollow))->SetDistances(arg0, arg1, arg2, arg3);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
