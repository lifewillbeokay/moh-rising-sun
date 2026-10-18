// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"
#include "AITargeting.h"

void BIFunc_AISetStatusVar(int **stack, void *object) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    aistatus_var var = (aistatus_var)*(*stack - (count - 1));
    int value = *(*stack - (count - 2));
    CAIObject *ai = ((ISceneNode *)object)->GetAIObject();
    int previous = ai->status.GetVarValue(var);
    ai->status.SetVarValue(var, value);
    if (var == 11 && value == 15 && previous != 15)
        ((CAIObjectInterface *)ai)->OnDeath();
    if (var == 11 && value == 15) {
        ISceneNode *anim = (ISceneNode *)((ISceneNode *)object)->AsAnimObject();
        if (anim && (anim->GetCollisionId() == 13 || anim->GetCollisionId() == 2))
            anim->SetCollisionId((EClsnId)25);
    }
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
