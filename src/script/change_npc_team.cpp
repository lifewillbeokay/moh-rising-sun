// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"
#include "SceneObjects.h"
#include "AITargeting.h"

void BIFunc_ChangeNPCTeam(int **stack, void *object) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int team = args[1];
    CAIObject *ai = ((ISceneNode *)object)->GetAIObject();
    ai->status.GetVarValue(aistatus_var_team);
    ai->status.SetVarValue(aistatus_var_team, team);
    ai->team = team;
    ((ISceneNode *)object)->SetCollisionId(GetCollisionIDFromScriptCollisionType((EScriptCollisionType)1, (ETeam)team));
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
