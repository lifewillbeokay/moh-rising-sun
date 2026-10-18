// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "CameraShake.h"

void BIFunc_MPGetPlayerVelocity(int **stack, void *) {
    float speed;
    int state;
    ((CAnimatedPlayer *)g_pBSObject->gameObject->GetSceneNode()->AsPlayerObject()->body)->GetFlocoParams(speed, state);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    float result = speed;
    **stack = *(volatile int *)&result;
}
