// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"
#include "CameraShake.h"
#include "AITargeting.h"

void BIFunc_ChangePlayersTeam(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int team = args[1];
    CAIObject *ai = ((ISceneNode *)g_scene.GetPlayer(0))->GetAIObject();
    ai->status.GetVarValue(aistatus_var_team);
    ai->status.SetVarValue(aistatus_var_team, team);
    ai->team = team;
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
