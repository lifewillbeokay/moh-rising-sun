// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "AITargeting.h"

void BIFunc_AICollisionTest(int **stack, void *object) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    ECollisionTestDirection direction = (ECollisionTestDirection)*(*stack - (count - 1));
    float distance = *(float *)(*stack - (count - 2));
    float clearance = ((ISceneNode *)object)->GetAIObject()->PerformCollisionTest(direction, distance, CAIObject::CollisionTestStyleDefault);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    volatile float result = clearance;
    **stack = *(int *)&result;
}

void BIFunc_Soldier_AICollisionTest(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *soldier = (BSObject *)*(*stack - (count - 1));
    ECollisionTestDirection direction = (ECollisionTestDirection)*(*stack - (count - 2));
    float distance = *(float *)(*stack - (count - 3));
    float clearance = soldier->gameObject->GetSceneNode()->GetAIObject()->PerformCollisionTest(direction, distance, CAIObject::CollisionTestStyleDefault);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    volatile float result = clearance;
    **stack = *(int *)&result;
}
