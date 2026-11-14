// AI-assisted GR8E69 reconstruction; see docs/research/frontend-screens.md.
#ifdef DECOMP_COMPARE
#include "PackageHandlers.h"
#include "GameSettings.h"
// Twelve-byte records in this file's private skinLock table; member names inferred.
struct FESkinLockEntry { int cheat, skin, team; };
extern FESkinLockEntry skinLock[];
extern int NumLockSkins;
int HandlerOnlineOptions::IsSkinUnlocked(int skin) {
    bool team = skin > 14;
    if (team) skin -= 15;
    for (int i = 0; i < NumLockSkins; ++i)
        if (skinLock[i].skin == skin && skinLock[i].team == team)
            return g_GameSettings.HasCheat(skinLock[i].cheat);
    return 1;
}
#endif
