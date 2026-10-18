// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "SceneObjects.h"

void BIFunc_DetachAllFromProp(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *prop = (BSObject *)args[1];
    if (!prop)
        prop = g_pBSObject;
    ((CAttachableObject *)prop->gameObject->GetSceneNode())->DetachAllObjects();
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
