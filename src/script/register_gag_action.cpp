// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "AIBehavior.h"

void BIFunc_RegisterGagAction(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    CScriptedGag::RegisterGagAction(g_pBSObject, (const char *)*(*stack - (count - 1)), *(*stack - (count - 2)));
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_StartGag(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    const char *gag = (const char *)*(*stack - (count - 1));
    int flags = *(*stack - (count - 2));
    BSObject *actor = (BSObject *)*(*stack - (count - 3));
    CScriptedGag::StartGag(actor, gag, flags);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_StartGagCRC(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    unsigned int gag = (unsigned int)*(*stack - (count - 1));
    int flags = *(*stack - (count - 2));
    BSObject *actor = (BSObject *)*(*stack - (count - 3));
    CScriptedGag::StartGag(actor, gag, flags);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
