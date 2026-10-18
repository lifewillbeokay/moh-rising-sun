// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"
#include "SceneObjects.h"

void BIFunc_ScaleRel(int **stack, void *object) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    float scale = *(float *)(*stack - (count - 1));
    float time = *(float *)(*stack - (count - 2));
    EBSEventEnum event = (EBSEventEnum)*(*stack - (count - 3));
    ((CStaticObjectInterface *)((ISceneNode *)object)->AsStaticObject())->ScaleFromScript(scale, time, event);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
