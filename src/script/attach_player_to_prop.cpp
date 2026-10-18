// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "SceneObjects.h"
#include "CameraShake.h"

void BIFunc_AttachPlayerToProp(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int slot = *(*stack - (count - 1));
    bool flag = *(bool *)(*stack - (count - 2));
    BSObject *prop = (BSObject *)*(*stack - (count - 3));
    if (!prop)
        prop = g_pBSObject;
    CAttachableObject *attachable = (CAttachableObject *)prop->gameObject->GetSceneNode();
    attachable->AttachObject(g_scene.GetPlayer(0), slot, flag);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_DetachFromProp(int **stack, void *object) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *other = (BSObject *)*(*stack - (count - 1));
    CAttachableObject *prop = (CAttachableObject *)((BSObject *)*(*stack - (count - 2)))->gameObject->GetSceneNode();
    ISceneNode *node;
    if (other)
        node = other->gameObject->GetSceneNode();
    else
        node = (ISceneNode *)object;
    prop->DetachObject(node);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
