// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"

int SetObjInfo(int, void *, void *, ISceneNode *);

void BIFunc_SetObjInfo(int **stack, void *object) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int result = SetObjInfo(*(*stack - (count - 1)), (void *)*(*stack - (count - 2)), (void *)*(*stack - (count - 3)), (ISceneNode *)object);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = result;
}

void BIFunc_Soldier_SetObjInfo(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *soldier = (BSObject *)*(*stack - (count - 1));
    int which = *(*stack - (count - 2));
    void *target = (void *)*(*stack - (count - 3));
    void *value = (void *)*(*stack - (count - 4));
    int result = SetObjInfo(which, target, value, soldier->gameObject->GetSceneNode());
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = result;
}
