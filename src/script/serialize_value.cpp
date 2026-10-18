// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "GameSettings.h"

void BIFunc_SerializeValue(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    unsigned int value = *(*stack - (count - 1));
    g_GameSettings.SerializeValue(value, *(*stack - (count - 2)));
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = value;
}
