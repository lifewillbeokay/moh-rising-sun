// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "AIBehavior.h"
#include "TriggerObject.h"

void BIFunc_GetBuddyLeadCheckpointFromID(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    TriggerObject *checkpoint = CBuddyLeadBehavior::GetBuddyLeadCheckpointFromID(args[1]);
    if (checkpoint) {
        *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
        **stack = (int)checkpoint->scriptObject;
    } else {
        *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
        **stack = 0;
    }
}
