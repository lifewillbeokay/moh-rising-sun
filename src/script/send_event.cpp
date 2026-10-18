// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "ScriptTimers.h"

void BSSendMessage(unsigned short, BSObject *, TriggerObject *, BSObject *, void *);

void BIFunc_SendEvent(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *target = (BSObject *)*(*stack - (count - 1));
    int event = *(*stack - (count - 2));
    void *data = (void *)*(*stack - (count - 3));
    if (target && target != (BSObject *)-1)
        BSObjectTriggerEvent(target, event, data, g_pBSObject, false);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_SendEventImmediate(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *target = (BSObject *)*(*stack - (count - 1));
    int event = *(*stack - (count - 2));
    void *data = (void *)*(*stack - (count - 3));
    if (target && target != (BSObject *)-1)
        BSObjectTriggerEvent(target, event, data, g_pBSObject, true);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_SendEventDelayed(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int delay = *(*stack - (count - 1));
    BSObject *target = (BSObject *)*(*stack - (count - 2));
    int event = *(*stack - (count - 3));
    void *data = (void *)*(*stack - (count - 4));
    BSRegisterTimerEvent(delay, event, target, data, TIMER_KEEP_EARLIER);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_SendMessage(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSSendMessage(*(*stack - (count - 1)), g_pBSObject, g_pBSObject->nativeObject, g_pBSObject,
                  (void *)*(*stack - (count - 2)));
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
