// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"

void BIFunc_Attach(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int result = 0;
    ISceneNode *parent = (ISceneNode *)*(*stack - (count - 1));
    ISceneNode *child = (ISceneNode *)*(*stack - (count - 2));
    unsigned int flags = *(*stack - (count - 3));
    if (child && parent)
        result = child->Attach(*parent, 0, flags);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = result;
}

void BIFunc_AttachChild(int **stack, void *object) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int result = 0;
    ISceneNode *child = (ISceneNode *)*(*stack - (count - 1));
    unsigned int flags = *(*stack - (count - 2));
    if (child && object)
        result = child->Attach(*(ISceneNode *)object, 0, flags);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = result;
}

void BIFunc_AttachParent(int **stack, void *object) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int result = 0;
    ISceneNode *parent = (ISceneNode *)*(*stack - (count - 1));
    unsigned int flags = *(*stack - (count - 2));
    if (object && parent)
        result = ((ISceneNode *)object)->Attach(*parent, 0, flags);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = result;
}
