// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "SceneObjects.h"
#include "CameraShake.h"

void BIFunc_AttachToProp(int **stack, void *object) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *other = (BSObject *)*(*stack - (count - 1));
    int slot = *(*stack - (count - 2));
    bool flag = *(bool *)(*stack - (count - 3));
    BSObject *propObject = (BSObject *)*(*stack - (count - 4));
    ISceneNode *node;
    if (other)
        node = other->gameObject->GetSceneNode();
    else
        node = (ISceneNode *)object;
    CAttachableObject *prop = (CAttachableObject *)propObject->gameObject->GetSceneNode();
    bool isAnim = node->AsAnimObject() != 0;
    bool isPlayer = node->AsPlayerObject() != 0;
    if (isPlayer)
        prop->AttachObject(node->AsPlayerObject(), slot, flag);
    else if (isAnim)
        prop->AttachObject(node->AsAnimObject(), slot);
    else
        prop->AttachObject(node->AsStaticObject(), slot);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_AttachAndAlignToProp(int **stack, void *object) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *other = (BSObject *)*(*stack - (count - 1));
    int slot = *(*stack - (count - 2));
    BSObject *propObject = (BSObject *)*(*stack - (count - 3));
    ISceneNode *node;
    if (other)
        node = other->gameObject->GetSceneNode();
    else
        node = (ISceneNode *)object;
    CAttachableObject *prop = (CAttachableObject *)propObject->gameObject->GetSceneNode();
    bool isAnim = node->AsAnimObject() != 0;
    if (isAnim)
        prop->AttachAndAlignObject(node->AsAnimObject(), slot);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
