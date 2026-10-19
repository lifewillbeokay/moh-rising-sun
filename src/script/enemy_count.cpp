// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "TriggerObject.h"

extern int g_iNumberOfEnemiesSpawned;
void DebugMsg(const char *, ...);

// Descriptive storage annotation for the shared property-name literal.
static const char teamField[]
    __attribute__((section(".rodata.team_field"), aligned(4))) = { 'T', 'e', 'a', 'm', 0 };

void BIFunc_ModifyTotalKillableEnemyCount(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int change = args[1];
    if (g_pBSObject->nativeObject->GetNativeBindingType() == 3 &&
            g_pBSObject->nativeObject->GetInt(teamField) == 2) {
        DebugMsg("-------------- Duplicate Creation:  Ignoring for total enemy count\n");
        g_iNumberOfEnemiesSpawned += change;
    }
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
