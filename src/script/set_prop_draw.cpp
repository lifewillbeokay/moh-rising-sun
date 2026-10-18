// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"
#include "SceneObjects.h"

void BIFunc_SetPropDraw(int **stack, void *object) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    bool enable = reinterpret_cast<bool *>(args)[1];
    ISceneNode *prop = ((ISceneNode *)object)->AsSoldierObject()->GetAttachedObjectAt("JO_prop");
    if (prop)
        prop->SetDrawEnabled(enable);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_SetDrawEnabled(int **stack, void *object) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    ISceneNode *node = (ISceneNode *)object;
    bool enable = reinterpret_cast<bool *>(args)[1];
    if (node) {
        node->SetDrawEnabled(enable);
        if (!enable)
            node->SetCollisionId((EClsnId)-1);
    }
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
