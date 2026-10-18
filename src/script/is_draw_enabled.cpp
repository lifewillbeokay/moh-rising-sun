// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"

void BIFunc_IsDrawEnabled(int **stack, void *object) {
    int enabled = 1;
    if (object)
        enabled = ((ISceneNode *)object)->IsDrawEnabled();
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = enabled;
}
