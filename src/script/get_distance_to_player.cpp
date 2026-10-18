// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "AIBehavior.h"

void BIFunc_GetDistanceToPlayer(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *buddy = (BSObject *)args[1];
    float distance = ((CBuddyFollowBehavior *)buddy->gameObject->GetSceneNode()->GetAIObject()->FindMatchingBehavior(BehaviorTypeBuddyFollow))->UpdateDistanceToPlayer();
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    volatile float result = distance;
    **stack = *(int *)&result;
}
