// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "CVector3.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "AITargeting.h"

void BIFunc_GetTargetRangeFor(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    ISceneNode *node = ((BSObject *)args[1])->gameObject->GetSceneNode();
    float range;
    if (!node->GetAIObject()->targeting.IsNonAITarget()) {
        ISceneNode *target = node->GetAIObject()->targeting.GetTargetScript()->gameObject->GetSceneNode();
        CVector3 targetPosition;
        CVector3 position;
        node->GetPosition(position);
        target->GetPosition(targetPosition);
        targetPosition -= position;
        range = sqrtf(targetPosition.LengthSquared());
    } else {
        CVector3 lastSeen;
        CVector3 position;
        node->GetPosition(position);
        node->GetAIObject()->targeting.GetLastSeenTargetPosition(lastSeen);
        position -= lastSeen;
        range = sqrtf(position.LengthSquared());
    }
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    volatile float result = range;
    **stack = *(int *)&result;
}
