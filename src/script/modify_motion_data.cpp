// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "SceneObjects.h"

void BIFunc_ModifyMotionData(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int index = *(*stack - (count - 1));
    BSObject *target = (BSObject *)*(*stack - (count - 2));
    const char *name = (const char *)*(*stack - (count - 3));
    if (!target)
        target = g_pBSObject;
    if (index < 0) {
        CStaticObject *object = target->gameObject->GetSceneNode()->AsStaticObject();
        object->StopMotionPlayback();
        object->ModifyMotionData(InitMotionData(name));
    } else {
        CHierObject *hier = target->gameObject->GetSceneNode()->AsHierObject();
        MotionData *data = InitMotionData(name);
        CStaticObject *sub = hier->GetSubObject(index);
        sub->StopMotionPlayback();
        ((CHierObject *)sub)->ModifyMotionData(data);
    }
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
