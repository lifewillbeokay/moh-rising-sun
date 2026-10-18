// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"

extern int g_CurrentHintPrompts[20];
extern int g_numHints;
extern int g_lastHintDisplayed;

void BIFunc_AddTriggeredHint(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int hint = args[1];
    if (hint) {
        if (g_numHints <= 19) {
            g_CurrentHintPrompts[g_numHints] = hint;
            g_numHints++;
        } else {
            g_CurrentHintPrompts[g_lastHintDisplayed] = hint;
        }
    }
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
