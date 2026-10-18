// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "CMatrix.h"
#include "TriggerObject.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "CameraShake.h"

void BIFunc_SetMMGLocation(int **stack, void *object) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *location = (BSObject *)args[1];
    CPlayerObject *player = ((ISceneNode *)object)->AsPlayerObject();
    CVector3 position;
    location->nativeObject->GetPosition(position);
    CMatrix matrix;
    if (!CMatrix::s_ClassInit)
        CMatrix::InitClass();
    matrix.BuildTrans(position);
    ((IMovingSceneNode *)player->body)->SetTMLocalToWorld(matrix);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
