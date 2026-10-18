// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"

void BIFunc_GetConstrainedRotateAngle(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    float angle = *(float *)(*stack - (count - 1));
    float current = *(float *)(*stack - (count - 2));
    float minimum = *(float *)(*stack - (count - 3));
    float maximum = *(float *)(*stack - (count - 4));
    if (!(current + angle <= maximum))
        angle = maximum - current;
    else if (!(current + angle >= minimum))
        angle = minimum - current;
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    volatile float result = angle;
    **stack = *(int *)&result;
}
