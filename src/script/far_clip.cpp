// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "Camera.h"
extern CCamera g_camera;
void BIFunc_SetFarClipPlane(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    g_camera.farClip = ((float *)args)[1];
    g_camera.worldToClipValid = g_camera.cameraToClipValid = 0;
    g_camera.field_160 = 1;
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
