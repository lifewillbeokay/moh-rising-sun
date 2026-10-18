// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "TriggerObject.h"
#include "SceneNode.h"

void MakeNoise(void *, CAIObject *, const CVector3 &, int, float, bool, int);

void BIFunc_MakeNoiseFromInputTrigger(int **stack, void *object) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    TriggerObject *trigger = (TriggerObject *)*(*stack - (count - 1));
    int loud = *(*stack - (count - 2));
    float radius = *(float *)(*stack - (count - 3));
    int type = 49;
    if (!loud)
        type = 48;
    loud = type;
    CVector3 position;
    trigger->GetPosition(position);
    MakeNoise(object, 0, position, loud, radius, true, 1);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
