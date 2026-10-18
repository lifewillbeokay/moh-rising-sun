// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"

void BSSendMessage(unsigned short, BSObject *, TriggerObject *, BSObject *, void *);

void BIFunc_SendMessageViaProxy(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *proxy = (BSObject *)*(*stack - (count - 1));
    int message = *(*stack - (count - 2));
    void *data = (void *)*(*stack - (count - 3));
    BSSendMessage(message, proxy, proxy->nativeObject, proxy, data);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
