// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"
#include "SceneObjects.h"

void BIFunc_SetMaxLightPriority(int **stack, void *object) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int priority = args[1];
    CStaticObject *staticObject = ((ISceneNode *)object)->AsStaticObject();
    if (staticObject)
        staticObject->maxLightPriority = priority;
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
