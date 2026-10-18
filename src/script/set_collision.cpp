// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "TriggerObject.h"
#include <stdio.h>
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "SceneObjects.h"
#include "CameraShake.h"
#include "AITargeting.h"

void BIFunc_SetCollision(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *target = (BSObject *)*(*stack - (count - 1));
    EScriptCollisionType type = (EScriptCollisionType)*(*stack - (count - 2));
    if (!target)
        target = g_pBSObject;
    ISceneNode *node = target->gameObject->GetSceneNode();
    ETeam team = (ETeam)0;
    switch (target->nativeObject->GetNativeBindingType()) {
    case 1:
    case 4:
    {
        ISceneNode *player = (ISceneNode *)node->AsPlayerObject();
        if (!player)
            player = (ISceneNode *)node->AsStaticObject();
        node = player;
    }
        break;
    case 3:
        node = (ISceneNode *)node->AsAnimObject();
        team = (ETeam)node->GetAIObject()->team;
        break;
    case 11:
        node = (ISceneNode *)node->AsAnimObject();
        break;
    case 2:
        break;
    default: {
        char message[256];
        sprintf(message, "Calling SetCollision() on unsupported type! - [%d]\n", target->nativeObject->GetNativeBindingType());
        break;
    }
    }
    node->SetCollisionId(GetCollisionIDFromScriptCollisionType(type, team));
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
