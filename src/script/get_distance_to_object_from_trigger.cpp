// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"

void BIFunc_GetDistanceToObjectFromTrigger(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *object = (BSObject *)args[1];
    CVector3 objectPosition;
    object->gameObject->GetSceneNode()->GetPosition(objectPosition);
    CVector3 triggerPosition;
    g_pBSObject->nativeObject->GetPosition(triggerPosition);
    float distance = triggerPosition.Distance(objectPosition);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    volatile float result = distance;
    **stack = *(int *)&result;
}
