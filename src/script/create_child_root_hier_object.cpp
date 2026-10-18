// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "SceneObjects.h"

void BIFunc_CreateChildRootHierObject(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    TriggerObject *trigger = g_pBSObject->nativeObject;
    int type = *(*stack - (count - 1));
    int flags = *(*stack - (count - 2));
    char *asset = (char *)*(*stack - (count - 3));
    int result = CreateRootHierObject(trigger, type, flags, AssetCRC(asset))[2];
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = result;
}

void BIFunc_CreateChildSubHierObject(int **stack, void *object) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int parent = *(*stack - (count - 1));
    int child = *(*stack - (count - 2));
    unsigned int attachment = *(*stack - (count - 3));
    int flags = *(*stack - (count - 4));
    char *asset = (char *)*(*stack - (count - 5));
    CHierObject *hier = ((ISceneNode *)object)->AsHierObject();
    CreateSubHierObject(hier, parent, child, attachment, flags, AssetCRC(asset));
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_CreateChildSubHierAnimObject(int **stack, void *object) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int parent = *(*stack - (count - 1));
    int child = *(*stack - (count - 2));
    unsigned int attachment = *(*stack - (count - 3));
    int flags = *(*stack - (count - 4));
    char *asset = (char *)*(*stack - (count - 5));
    char *animation = (char *)*(*stack - (count - 6));
    CHierObject *hier = ((ISceneNode *)object)->AsHierObject();
    CreateSubHierAnimObject(hier, parent, child, attachment, flags, AssetCRC(asset), AssetCRC(animation));
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
