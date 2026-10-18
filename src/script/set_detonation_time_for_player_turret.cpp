// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"
#include "CameraShake.h"

void BIFunc_SetDetonationTimeForPlayerTurret(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    float seconds = reinterpret_cast<float *>(args)[1];
    g_scene.GetPlayer(0)->SetDetonationTimeForPlayerTurret(seconds * 30.0f);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
