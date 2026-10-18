// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"
#include "AITargeting.h"

void BIFunc_SetInShadow(int **stack, void *object) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int inShadow = *(*stack - (count - 1));
    float amount = *(float *)(*stack - (count - 2));
    if (object) {
        CAIFilterObject *filter = ((ISceneNode *)object)->GetAIDoodad()->filter;
        if (filter) {
            CAIObject *ai = filter->object;
            ai->inShadow = inShadow;
            if (inShadow)
                ai->shadowAmount = amount;
            else
                ai->shadowAmount = 1.0f;
        }
    }
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
