// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "SceneObjects.h"

void BIFunc_TranslateRel(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int index = *(*stack - (count - 1));
    float x = *(float *)(*stack - (count - 2));
    float y = *(float *)(*stack - (count - 3));
    float z = *(float *)(*stack - (count - 4));
    float vx = *(float *)(*stack - (count - 5));
    float vy = *(float *)(*stack - (count - 6));
    float vz = *(float *)(*stack - (count - 7));
    EBSEventEnum event = (EBSEventEnum)*(*stack - (count - 8));
    CVector3 offset(x, y, z);
    CVector3 velocity(vx, vy, vz);
    if (index != -1)
        ((CStaticObjectInterface *)g_pBSObject->gameObject->GetSceneNode()->AsHierObject()->GetSubObject(index))
            ->TranslateFromScript(offset, velocity, event);
    else
        ((CStaticObjectInterface *)g_pBSObject->gameObject->GetSceneNode()->AsStaticObject())
            ->TranslateFromScript(offset, velocity, event);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_RotateRel(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int index = *(*stack - (count - 1));
    float x = *(float *)(*stack - (count - 2)) * (3.141593f / 180.0f);
    float y = *(float *)(*stack - (count - 3)) * (3.141593f / 180.0f);
    float z = *(float *)(*stack - (count - 4)) * (3.141593f / 180.0f);
    float vx = *(float *)(*stack - (count - 5)) * (3.141593f / 180.0f);
    float vy = *(float *)(*stack - (count - 6)) * (3.141593f / 180.0f);
    float vz = *(float *)(*stack - (count - 7)) * (3.141593f / 180.0f);
    EBSEventEnum event = (EBSEventEnum)*(*stack - (count - 8));
    CVector3 offset(x, y, z);
    CVector3 velocity(vx, vy, vz);
    if (index != -1)
        ((CStaticObjectInterface *)g_pBSObject->gameObject->GetSceneNode()->AsHierObject()->GetSubObject(index))
            ->RotateFromScript(offset, velocity, event);
    else
        ((CStaticObjectInterface *)g_pBSObject->gameObject->GetSceneNode()->AsStaticObject())
            ->RotateFromScript(offset, velocity, event);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
