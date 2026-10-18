// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "SceneObjects.h"

void BIFunc_HierObjectPlayAnimation(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int index = *(*stack - (count - 1));
    int animation = *(*stack - (count - 2));
    int start = *(*stack - (count - 3));
    int flags = *(*stack - (count - 4));
    EBSEventEnum event = (EBSEventEnum)*(*stack - (count - 5));
    g_pBSObject->gameObject->GetSceneNode()->AsHierObject()->GetSubObject(index)->PlayAnimation(animation, start, flags, event);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_StartMotionPlayback(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int index = *(*stack - (count - 1));
    int motion = *(*stack - (count - 2));
    int flags = *(*stack - (count - 3));
    EBSEventEnum event = (EBSEventEnum)*(*stack - (count - 4));
    BSObject *target = (BSObject *)*(*stack - (count - 5));
    if (!target)
        target = g_pBSObject;
    if (index < 0)
        ((CStaticObjectInterface *)target->gameObject->GetSceneNode()->AsStaticObject())->StartMotionPlayback(motion, flags, event);
    else
        ((CStaticObjectInterface *)target->gameObject->GetSceneNode()->AsHierObject()->GetSubObject(index))->StartMotionPlayback(motion, flags, event);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
