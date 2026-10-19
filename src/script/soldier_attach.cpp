// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "SceneObjects.h"
#include "TriggerObject.h"

void DebugMsg(const char *, ...);

// Descriptive storage annotations for literals shared with original context.
static const char leftHand[]
    __attribute__((section(".rodata.left_hand"), aligned(4))) = "lt_hand";
static const char rightHand[]
    __attribute__((section(".rodata.right_hand"), aligned(4))) = "rt_hand";
static const char idField[]
    __attribute__((section(".rodata.id_field"), aligned(4))) = { 'I', 'D', 0 };

void BIFunc_AttachToSoldier(int **stack, void *object) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *item = (BSObject *)*(*stack - (count - 1));
    int hand = *(*stack - (count - 2));
    int flags = *(*stack - (count - 3));
    ISceneNode *node = item->gameObject->GetSceneNode();
    switch (hand) {
    case 0:
        ((CAnimObjectInterface *)object)->AttachObject((CStaticObject *)node,
            (char *)leftHand, flags);
        break;
    case 1:
        ((CAnimObjectInterface *)object)->AttachObject((CStaticObject *)node,
            (char *)rightHand, flags);
        break;
    }
    g_scene.Remove(*node);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_AttachToSoldierEmptyHand(int **stack, void *object) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *item = (BSObject *)*(*stack - (count - 1));
    int flags = *(*stack - (count - 2));
    int result = -1;
    ISceneNode *node = item->gameObject->GetSceneNode();
    if (!g_scene.IsNodeInScene(node)) {
        if (item)
            DebugMsg("Prop %d has already been attached or destroyed. Check with Designers for possible duplicate ID.\n",
                item->nativeObject->GetInt(idField));
    } else {
        if (!((CAnimObjectInterface *)object)->AttachObject((CStaticObject *)node,
                (char *)rightHand, flags)) {
            if (!((CAnimObjectInterface *)object)->AttachObject((CStaticObject *)node,
                    (char *)leftHand, flags))
                goto done;
            result = 0;
        } else {
            result = 1;
        }
        g_scene.Remove(*node);
    }
done:
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = result;
}
