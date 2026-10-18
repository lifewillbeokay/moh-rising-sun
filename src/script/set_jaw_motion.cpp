// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"
#include "SceneObjects.h"

void BIFunc_SetJawMotion(int **stack, void *object) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    EJawType type = (EJawType)args[1];
    ((ISceneNode *)object)->AsSoldierObject()->jaw.SetMotionType(type);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
