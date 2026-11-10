// HandlerBonusOptions, the bonus screen: cheat accessors; see docs/Frontend.md.
#include "PackageHandlers.h"
#include "GameSettings.h"
#include <stdio.h>
void DebugMsg(const char *, ...);
bool HandlerBonusOptions::GetCheatByIndex(int index) {
    switch (index) {
    case 0: return g_cheats.allReplayItems;
    case 1: return g_cheats.infiniteAmmo;
    case 2: return g_cheats.oneShotKillsEnemies;
    case 3: return g_cheats.unknown_14;
    case 4: return g_cheats.keepGrenadeSpeed;
    case 5: return g_cheats.unknown_18;
    case 6: return g_cheats.unknown_8;
    case 7: return g_cheats.headShotsOnly;
    case 8: return g_cheats.unknown_2c;
    case 9: return g_cheats.oneHitKills;
    case 10: return g_cheats.invincible;
    case 11: return g_cheats.unknown_c;
    default:
        DebugMsg("Illegal index in GetCheatByIndex: %d\n", index);
        return false;
    }
}
void HandlerBonusOptions::SetCheatByIndex(int index, bool value) {
    switch (index) {
    case 0: g_cheats.allReplayItems = value; break;
    case 1: g_cheats.infiniteAmmo = value; break;
    case 2: g_cheats.oneShotKillsEnemies = value; break;
    case 3: g_cheats.unknown_14 = value; break;
    case 4: g_cheats.keepGrenadeSpeed = value; break;
    case 5: g_cheats.unknown_18 = value; break;
    case 6: g_cheats.unknown_8 = value; break;
    case 7: g_cheats.headShotsOnly = value; break;
    case 8: g_cheats.unknown_2c = value; break;
    case 9: g_cheats.oneHitKills = value; break;
    case 10: g_cheats.invincible = value; break;
    case 11: g_cheats.unknown_c = value; break;
    default:
        DebugMsg("Illegal index in GetCheatByIndex: %d\n", index);
        return;
    }
    g_GameSettings.ActivateCheat(index, value);
}
bool HandlerBonusOptions::ToggleCheatByIndex(int index) {
    bool value = !GetCheatByIndex(index);
    SetCheatByIndex(index, value);
    return value;
}
