// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "AITargeting.h"

void BIFunc_IsNearbyPositionOnAStarMesh(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *soldier = (BSObject *)*(*stack - (count - 1));
    ECollisionTestDirection arg0 = (ECollisionTestDirection)*(*stack - (count - 2));
    float arg1 = *(float *)&*(*stack - (count - 3));
    int result = soldier->gameObject->GetSceneNode()->GetAIObject()->IsNearbyPositionOnAStarMesh(arg0, arg1);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = result;
}
