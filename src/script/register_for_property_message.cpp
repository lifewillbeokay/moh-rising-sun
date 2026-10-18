// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"

void BSRegisterMessageToEvent(unsigned short, unsigned short, BSObject *);
void BSUnregisterMessageToEvent(unsigned short, unsigned short, BSObject *);

void BIFunc_RegisterForPropertyMessage(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int property = *(*stack - (count - 1));
    int event = *(*stack - (count - 2));
    BSRegisterMessageToEvent(event, property, g_pBSObject);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_StopPropertyMessage(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int property = *(*stack - (count - 1));
    int event = *(*stack - (count - 2));
    BSUnregisterMessageToEvent(event, property, g_pBSObject);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
