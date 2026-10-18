// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "CameraShake.h"

int IsPlayerHoldingWeapon();

void BIFunc_IsPlayerDisguised(int **stack, void *) {
    BSObject *script = (BSObject *)((ISceneNode *)g_scene.GetPlayer(0))->GetScriptObject();
    int *data = (int *)script->gameObject->GetScriptData();
    int disguised = 0;
    if (data[4] && data[5])
        disguised = !IsPlayerHoldingWeapon();
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = disguised;
}
