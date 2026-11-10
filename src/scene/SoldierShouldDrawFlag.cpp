// AI-assisted reconstruction from GR8E69. Part of the original Soldier_object.cpp; there is no
// Frontline counterpart. The flag shows to the player of the carrier's team who owns this view.
#include "SceneObjects.h"

extern int g_bInMultiplayerMode;
extern int g_bTeamDM;
extern int g_bMPEndLevel;

// Only what this test reads; the owner's identity is compared by value. Names inferred.
struct FlagDrawView {
    unsigned char unknown_0[0x18];
    int owner; // +0x18
};
struct FlagDrawContext {
    unsigned char unknown_0[0x114];
    FlagDrawView *view; // +0x114
};
struct FlagPlayerOwner {
    unsigned char unknown_0[0x10d8];
    int owner; // +0x10d8
};

int CSoldierObject::ShouldDrawFlag(CDrawContext &context) {
    if (flagTeam > -1 && g_bInMultiplayerMode && g_bTeamDM && !g_bMPEndLevel) {
        int count = g_scene.playerCount;
        for (int i = 0; i < count; i++) {
            CPlayerObject *player = g_scene.GetPlayer(i);
            if (((FlagPlayerOwner *)player)->owner == ((FlagDrawContext *)&context)->view->owner)
                return ((IMovingSceneNode *)player)->GetTeam() == flagTeam;
        }
    }
    return 0;
}
