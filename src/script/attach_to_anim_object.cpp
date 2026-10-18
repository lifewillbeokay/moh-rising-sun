// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "SceneObjects.h"

void BIFunc_AttachToAnimObject(int **stack, void *object) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *item = (BSObject *)*(*stack - (count - 1));
    int mode = *(*stack - (count - 2));
    int flags = *(*stack - (count - 3));
    ISceneNode *node = item->gameObject->GetSceneNode();
    if (mode == 0) {
        ((CAnimObjectInterface *)object)->AttachObject((CStaticObject *)node, "JO_howdah", flags);
    }
    g_scene.Remove(*node);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
