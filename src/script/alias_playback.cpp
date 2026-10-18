// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SoundManager.h"
#include "StringCRC.h"
#include "SceneNode.h"

void BIFunc_PlayAliasSound(int **stack, void *object) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    unsigned int sound = *(*stack - (count - 1));
    int flags = *(*stack - (count - 2));
    CVector3 offset(0.0f, 0.0f, 0.0f);
    int handle;
    if (GetStringCRC("none") != sound)
        handle = PlaySoundFromTriggerOrObject2(g_pBSObject->nativeObject,
            g_pBSObject, object, sound, flags, offset, false);
    else
        handle = -1;
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = handle;
}

void BIFunc_PlayAliasSoundOnOther(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *target = (BSObject *)*(*stack - (count - 1));
    unsigned int sound = *(*stack - (count - 2));
    int flags = *(*stack - (count - 3));
    CVector3 offset(0.0f, 0.0f, 0.0f);
    ISceneNode *node = 0;
    TriggerObject *trigger = 0;
    if (target) {
        trigger = target->nativeObject;
        if (target->gameObject)
            node = target->gameObject->GetSceneNode();
    }
    int handle = -1;
    if (trigger || node)
        handle = PlaySoundFromTriggerOrObject2(trigger, g_pBSObject, node,
            sound, flags, offset, false);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = handle;
}

void BIFunc_PlayVocal(int **stack, void *object) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    unsigned int sound = *(*stack - (count - 1));
    int flags = *(*stack - (count - 2));
    CVector3 offset(0.0f, 0.0f, 0.0f);
    int handle;
    if (GetStringCRC("none") != sound)
        handle = PlaySoundFromTriggerOrObject2(g_pBSObject->nativeObject,
            g_pBSObject, object, sound, flags, offset, true);
    else
        handle = -1;
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = handle;
}
