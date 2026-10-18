// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SoundManager.h"

void BIFunc_PlaySoundFromInputTrigger2(int **stack, void *object) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    TriggerObject *trigger = (TriggerObject *)*(*stack - (count - 1));
    unsigned int sound = *(*stack - (count - 2));
    int flags = *(*stack - (count - 3));
    CVector3 offset(0.0f, 0.0f, 0.0f);
    int handle = PlaySoundFromTriggerOrObject2(trigger, g_pBSObject, object, sound, flags, offset, false);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = handle;
}

void BIFunc_PlaySoundAtOffset2(int **stack, void *object) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    unsigned int sound = *(*stack - (count - 1));
    int flags = *(*stack - (count - 2));
    CVector3 offset(*(float *)(*stack - (count - 3)), *(float *)(*stack - (count - 4)), *(float *)(*stack - (count - 5)));
    int handle = PlaySoundFromTriggerOrObject2(g_pBSObject->nativeObject, g_pBSObject, object, sound, flags, offset, false);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = handle;
}
