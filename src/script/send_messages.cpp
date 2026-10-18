// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"

void BSSendMessage(unsigned short, BSObject *, TriggerObject *, BSObject *, void *);

void BIFunc_SendMessages(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int *messages = (int *)*(*stack - (count - 1));
    BSObject *target = (BSObject *)*(*stack - (count - 2));
    void *data = (void *)*(*stack - (count - 3));
    if (!target)
        target = g_pBSObject;
    int total = messages[0];
    for (int i = 0; i < total; i++) {
        int message = messages[i + 1];
        BSSendMessage(message, g_pBSObject, g_pBSObject->nativeObject, target, data);
    }
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
