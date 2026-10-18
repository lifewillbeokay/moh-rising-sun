// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"
#include "AITargeting.h"

void BIFunc_AISendFlag(int **stack, void *object) {
    CAIObject *ai = ((ISceneNode *)object)->GetAIObject();
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int flag = *(*stack - (count - 1));
    switch (flag) {
    default:
        ai->statusFlags |= flag;
        break;
    case 1:
        ai->targeting.alerted = flag;
        break;
    case 2:
        if (*(*stack - (count - 2)))
            ai->statusFlags |= 2;
        else {
            ai->statusFlags &= ~2;
            ai->statusFlags2 &= ~1;
        }
        break;
    case 4:
        ai->statusFlags |= 4;
        break;
    case 8:
        if (*(*stack - (count - 2)))
            ai->statusFlags |= 8;
        else
            ai->statusFlags &= ~8;
        break;
    }
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
