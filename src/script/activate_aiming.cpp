// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"
#include "SceneObjects.h"

void BIFunc_ActivateAiming(int **stack, void *object) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    bool enable = reinterpret_cast<bool *>(args)[1];
    ((CAnimObjectInterface *)((ISceneNode *)object)->AsSoldierObject())->EnableAimModifier(enable);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
