// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"
#include "AITargeting.h"

void BIFunc_AISnapToMovePoint(int **stack, void *object) {
    CAIObject *ai = ((ISceneNode *)object)->GetAIObject();
    CAIObject::SnapState *snap = &ai->snap;
    snap->position = ai->locomotion.arbitraryPosition;
    snap->useAngle = 0;
    snap->active = 1;
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_AISnapToMovePointAndAngle(int **stack, void *object) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    float angle = *(float *)&args[1];
    CAIObject *ai = ((ISceneNode *)object)->GetAIObject();
    CAIObject::SnapState *snap = &ai->snap;
    snap->position = ai->locomotion.arbitraryPosition;
    snap->useAngle = 1;
    snap->angle = angle;
    snap->active = 1;
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
