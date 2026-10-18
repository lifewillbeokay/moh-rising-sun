// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneObjects.h"
#include "Camera.h"

void BIFunc_DoSomethingInteresting(int **stack, void *object) {
    CVector3 position;
    g_camera.GetPosition(position);
    ((CAnimObjectInterface *)object)->SetMoveTarget(position);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
