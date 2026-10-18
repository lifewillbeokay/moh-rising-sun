// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"

void BIFunc_SetLightVolumeTransitionDuration(int **stack, void *object) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *target = (BSObject *)*(*stack - (count - 1));
    float duration = *(float *)(*stack - (count - 2));
    ISceneNode *node;
    if (target)
        node = target->gameObject->GetSceneNode();
    else
        node = (ISceneNode *)object;
    IMovingSceneNode *moving = node->AsMovingNode();
    if (moving)
        moving->SetLightVolumeTransitionDuration(duration);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
