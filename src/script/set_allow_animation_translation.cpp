// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "SceneObjects.h"

void BIFunc_SetAllowAnimationTranslation(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *target = (BSObject *)*(*stack - (count - 1));
    int allow = *(*stack - (count - 2));
    if (!target)
        target = g_pBSObject;
    if (target && target->gameObject && target->gameObject->GetSceneNode() && target->gameObject->GetSceneNode()->AsAnimObject())
        target->gameObject->GetSceneNode()->AsAnimObject()->allowAnimationTranslation = allow;
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
