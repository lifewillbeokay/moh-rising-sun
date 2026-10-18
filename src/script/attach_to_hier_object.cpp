// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "SceneObjects.h"
#include "CameraShake.h"

void BIFunc_AttachToHierObject(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    CHierObject *hier = 0;
    BSObject *child = (BSObject *)*(*stack - (count - 1));
    BSObject *hierObject = (BSObject *)*(*stack - (count - 2));
    int index = *(*stack - (count - 3));
    int slot = *(*stack - (count - 4));
    if (hierObject && hierObject->gameObject && hierObject->gameObject->GetSceneNode())
        hier = hierObject->gameObject->GetSceneNode()->AsHierObject();
    CAttachableObject *sub = (CAttachableObject *)hier->GetSubObject(index);
    if (sub && child && child->gameObject && child->gameObject->GetSceneNode()) {
        ISceneNode *node = child->gameObject->GetSceneNode();
        bool isAnim = node->AsAnimObject() != 0;
        bool isPlayer = node->AsPlayerObject() != 0;
        if (isPlayer)
            sub->AttachObject(node->AsPlayerObject(), slot, true);
        else if (isAnim)
            sub->AttachObject(node->AsAnimObject(), slot);
        else
            sub->AttachObject(node->AsStaticObject(), slot);
    }
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
