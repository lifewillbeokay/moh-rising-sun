// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"

long long MathFunRandomI64(long long, long long);

void BIFunc_GetRandomInteger(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int low = *(*stack - (count - 1));
    int high = *(*stack - (count - 2));
    int value;
    if (low == high)
        value = high;
    else
        value = MathFunRandomI64(low, high);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = value;
}
