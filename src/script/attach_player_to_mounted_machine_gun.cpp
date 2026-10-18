// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"
#include "SceneObjects.h"
#include "CameraShake.h"
#include "AIBehavior.h"

extern int g_bCoopMultiplayer;
extern int g_bInMultiplayerMode;

void BIFunc_AttachPlayerToMountedMachineGun(int **stack, void *object) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int linkCode = *(*stack - (count - 1));
    bool attach = *(bool *)(*stack - (count - 2));
    CStaticObject *gun = CMountedMachineGunBehavior::GetGunByLinkCode(linkCode);
    if (g_bInMultiplayerMode || g_bCoopMultiplayer) {
        CPlayerObject *player = ((ISceneNode *)object)->AsPlayerObject();
        if (!player) {
            CVector3 position;
            ((ISceneNode *)object)->GetPosition(position);
            player = g_scene.FindClosestPlayer(position);
        }
        if (!player->pathBits.mmgBlocked)
            player->SetUsingMountedMachineGun(attach, gun);
    } else
        g_scene.GetPlayer(0)->SetUsingMountedMachineGun(attach, gun);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
