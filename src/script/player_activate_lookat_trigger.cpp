// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "CameraShake.h"

void BIFunc_PlayerActivateLookatTrigger(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    float time = *(float *)(*stack - (count - 1));
    float angle = *(float *)(*stack - (count - 2));
    g_scene.GetPlayer(0)->HandleLookatTrigger(g_pBSObject, time, angle);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_PlayerDeactivateLookatTrigger(int **stack, void *) {
    g_scene.GetPlayer(0)->HandleLookatTrigger(0, 0.0f, 0.0f);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_PlayerStartLookAt(int **stack, void *object) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *target = (BSObject *)*(*stack - (count - 1));
    float time = *(float *)(*stack - (count - 2));
    if (target == 0 || target == (BSObject *)-1)
        target = (BSObject *)object;
    if (target) {
        if (target->gameObject) {
            ISceneNode *node = target->gameObject->GetSceneNode();
            g_scene.GetPlayer(0)->lookAt.Enable(node, time);
        } else {
            CVector3 position;
            target->nativeObject->GetPosition(position);
            g_scene.GetPlayer(0)->lookAt.Enable(position, time);
        }
    } else {
        CVector3 position;
        g_pBSObject->nativeObject->GetPosition(position);
        g_scene.GetPlayer(0)->lookAt.Enable(position, time);
    }
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
