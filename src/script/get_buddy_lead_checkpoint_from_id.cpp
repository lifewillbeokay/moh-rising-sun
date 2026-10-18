// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "AIBehavior.h"

void BIFunc_GetBuddyLeadCheckpointFromID(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BuddyLeadCheckpoint *checkpoint = CBuddyLeadBehavior::GetBuddyLeadCheckpointFromID(args[1]);
    if (checkpoint) {
        *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
        **stack = checkpoint->value;
    } else {
        *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
        **stack = 0;
    }
}
