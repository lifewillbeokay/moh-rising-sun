// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"

void DelayDestroyObject(BSGO_Basic *);
void DestroySubHierObject(CHierObject *, int);

void BIFunc_DestroySubHierObject(int **stack, void *object) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int index = args[1];
    CHierObject *hier = ((ISceneNode *)object)->AsHierObject();
    if (index == 0)
        DelayDestroyObject(g_pBSObject->gameObject.operator->());
    else
        DestroySubHierObject(hier, index);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
