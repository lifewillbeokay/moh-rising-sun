// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"
#include "SceneObjects.h"

void BIFunc_MPSetAiming(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    ISceneNode *node = (ISceneNode *)*(*stack - (count - 1));
    bool enable = *(bool *)(*stack - (count - 2));
    ((CAnimObjectInterface *)node->AsAnimatedPlayerObject())->EnableAimModifier(enable);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
