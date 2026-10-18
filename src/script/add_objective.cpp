// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "Objectives.h"

extern int g_iHighestObjectiveNumber;

void BIFunc_AddObjective(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int objective = *(*stack - (count - 1));
    g_Objectives.AddObjective(objective, *(*stack - (count - 2)), *(bool *)(*stack - (count - 3)),
                              *(bool *)(*stack - (count - 4)), *(bool *)(*stack - (count - 5)));
    if (objective > g_iHighestObjectiveNumber)
        g_iHighestObjectiveNumber = objective;
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_ChangeObjectivePrompt(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int objective = *(*stack - (count - 1));
    unsigned int prompt = *(*stack - (count - 2));
    if (objective <= g_iHighestObjectiveNumber)
        g_Objectives.ChangeObjectivePrompt(objective, prompt);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
