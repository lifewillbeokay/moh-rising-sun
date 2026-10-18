// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "CameraShake.h"

void BIFunc_MPGetPlayerBody(int **stack, void *) {
    CPlayerObject *player = g_pBSObject->gameObject->GetSceneNode()->AsPlayerObject();
    int body = player->body;
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = body;
}
