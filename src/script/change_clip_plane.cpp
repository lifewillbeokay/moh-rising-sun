// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"
#include "CameraShake.h"

void BIFunc_ChangeClipPlane(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    float farClip = reinterpret_cast<float *>(args)[1];
    ClipSettings *clip = &g_scene.GetPlayer(0)->clip;
    clip->farClip = farClip;
    clip->field_158 = 0;
    clip->field_15c = 0;
    clip->changed = 1;
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
