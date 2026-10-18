// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"
#include "SceneObjects.h"

void BIFunc_SuspendAiming(int **stack, void *object) {
    ((ISceneNode *)object)->AsSoldierObject()->aimSuspended = 1;
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_ResumeAiming(int **stack, void *object) {
    ((ISceneNode *)object)->AsSoldierObject()->ResumeAimModifier();
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
