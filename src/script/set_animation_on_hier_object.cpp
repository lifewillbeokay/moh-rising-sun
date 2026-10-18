// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "SceneObjects.h"

void BIFunc_SetAnimationOnHierObject(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int index = *(*stack - (count - 1));
    const char *state = (const char *)*(*stack - (count - 2));
    int flags = *(*stack - (count - 3));
    CStaticObject *sub = g_pBSObject->gameObject->GetSceneNode()->AsHierObject()->GetSubObject(index);
    ((CStaticObjectInterface *)sub)->SetAnimationState(state, flags);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
