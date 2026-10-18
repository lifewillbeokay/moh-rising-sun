// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"

extern int g_CurrentHintPrompts[20];
extern int g_numHints;

void BIFunc_RemoveTriggeredHint(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int hint = args[1];
    if (hint) {
        int found = 0;
        for (int i = 0; i < g_numHints; i++) {
            if (!found && g_CurrentHintPrompts[i] == hint)
                found = 1;
            if (found && i != g_numHints - 1)
                g_CurrentHintPrompts[i] = g_CurrentHintPrompts[i + 1];
        }
        if (found)
            g_numHints--;
    }
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
