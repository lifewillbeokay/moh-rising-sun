// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "SceneObjects.h"

void BIFunc_Serializable(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int result = 0;
    BSObject *object = (BSObject *)args[1];
    if (object && object->gameObject && object->gameObject->GetSceneNode())
        result = 1;
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = result;
}

void BIFunc_SerializeData(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *object = (BSObject *)args[1];
    if (object->gameObject->GetSceneNode())
        object->gameObject->GetSceneNode()->SerializeSaveData();
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_SaveGagState(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *object = (BSObject *)args[1];
    ISceneNode *node = object->gameObject->GetSceneNode();
    if (node->AsSoldierObject())
        node->AsSoldierObject()->SaveGagState();
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
