// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "SceneObjects.h"

void BIFunc_SetAnimCallback(int **stack, void *object) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    ((CAnimObjectInterface *)object)->SetAnimCallback((EAnimCallback)args[1]);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_SetAnimCallbackOnOther(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *other = (BSObject *)*(*stack - (count - 1));
    EAnimCallback callback = (EAnimCallback)*(*stack - (count - 2));
    ((CAnimObjectInterface *)other->gameObject->GetSceneNode())->SetAnimCallback(callback);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
