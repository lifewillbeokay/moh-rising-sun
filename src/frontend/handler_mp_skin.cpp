// AI-assisted GR8E69 reconstruction; see docs/research/frontend-screens.md.
#include "PackageHandlers.h"
#include "GameSettings.h"
// Twelve-byte records in this file's private skinLock table; member names inferred.
struct FESkinLockEntry { int cheat, skin, team; };
extern FESkinLockEntry skinLock[];
extern int NumLockSkins;
int HandlerMPBots::IsSkinUnlocked(int skin, int team) {
    for (int i = 0; i < NumLockSkins; ++i) {
        if (team == -1) {
            int index = skinLock[i].skin;
            if (skinLock[i].team == 1) index += 15;
            if (index == skin) return g_GameSettings.HasCheat(skinLock[i].cheat);
        } else if (skinLock[i].skin == skin && skinLock[i].team == team)
            return g_GameSettings.HasCheat(skinLock[i].cheat);
    }
    return 1;
}
