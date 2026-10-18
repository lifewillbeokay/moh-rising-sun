// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"

void BIFunc_BSObject_AsSceneNode(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *target = (BSObject *)args[1];
    ISceneNode *node = 0;
    if (target->gameObject)
        node = target->gameObject->GetSceneNode();
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = (int)node;
}
