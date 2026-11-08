// AI-assisted partial reconstruction from GR8E69; see docs/Script.md.
#if defined(DECOMP_COMPARE)
#include "CameraShake.h"
#include "SceneNode.h"
#include "SceneObjects.h"
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "Shell.h"

extern int g_bInMultiplayerMode;

void BIFunc_PlayVibrationEvent(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int effect = *(*stack - (count - 1));
    float duration = *(float *)(*stack - (count - 2));
    float strength = *(float *)(*stack - (count - 3));
    int flags = *(*stack - (count - 4));
    int player;
    if (g_bInMultiplayerMode) {
        if (!g_pBSObject->gameObject->GetSceneNode()->AsPlayerObject()) {
            CVector3 position;
            g_pBSObject->gameObject->GetSceneNode()->GetPosition(position);
            player = ((IMovingSceneNode *)g_scene.FindClosestPlayer(position))->GetPlayerIndex();
        } else {
            player = ((IMovingSceneNode *)g_pBSObject->gameObject->GetSceneNode()->AsPlayerObject())
                         ->GetPlayerIndex();
        }
    } else {
        player = ((IMovingSceneNode *)g_scene.GetPlayer(0))->GetPlayerIndex();
    }
    g_inputMgr.SetFeedback(player, effect, flags, strength, duration);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

#endif
