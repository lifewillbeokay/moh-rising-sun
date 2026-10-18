// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "SceneObjects.h"

void BIFunc_StaticObjectPlayAnimation(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int animation = *(*stack - (count - 1));
    int start = *(*stack - (count - 2));
    int flags = *(*stack - (count - 3));
    EBSEventEnum event = (EBSEventEnum)*(*stack - (count - 4));
    g_pBSObject->gameObject->GetSceneNode()->AsStaticObject()->PlayAnimation(animation, start, flags, event);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
