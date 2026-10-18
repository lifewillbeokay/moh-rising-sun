// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SoundManager.h"

void BIFunc_PlaySoundFromInputTrigger(int **stack, void *object) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    TriggerObject *trigger = (TriggerObject *)*(*stack - (count - 1));
    int sound = *(*stack - (count - 2));
    int flags = *(*stack - (count - 3));
    CVector3 offset(0.0f, 0.0f, 0.0f);
    int handle = PlaySoundFromTriggerOrObject(trigger, g_pBSObject, object, sound, flags, offset);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = handle;
}

void BIFunc_PlaySoundAtOffset(int **stack, void *object) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int sound = *(*stack - (count - 1));
    int flags = *(*stack - (count - 2));
    CVector3 offset(*(float *)(*stack - (count - 3)), *(float *)(*stack - (count - 4)), *(float *)(*stack - (count - 5)));
    int handle = PlaySoundFromTriggerOrObject(g_pBSObject->nativeObject, g_pBSObject, object, sound, flags, offset);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = handle;
}

void BIFunc_PlaySound(int **stack, void *object) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int sound = *(*stack - (count - 1));
    int flags = *(*stack - (count - 2));
    int handle = -12;
    CVector3 offset(0.0f, 0.0f, 0.0f);
    if (sound != -1)
        handle = PlaySoundFromTriggerOrObject(g_pBSObject->nativeObject, g_pBSObject, object, sound, flags, offset);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = handle;
}
