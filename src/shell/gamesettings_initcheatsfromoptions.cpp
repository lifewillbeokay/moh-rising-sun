// CGameSettings: options, save data and the profile checksum.
#include "GameSettingsViews.h"
#include "CRC.h"
#include "Objectives.h"
#include "Shell.h"
#include "Toilet.h"
extern KEYMAP g_keymap[64];
extern KEYMAP g_config_keymap[4][64];
extern "C" void *DWI_alloc(const char *, int, int);
inline void *operator new(unsigned int, void *p) { return p; }
// A resource record holding a counted list of compartment IDs.
struct CompartmentListEntry { int unknown_0; unsigned int count; int ids[1]; };

void CGameSettings::InitCheatsFromOptions() {
    if (g_GameSettings.HasCheat(0)) {
        if (g_GameSettings.IsCheatActive(0)) g_cheats.allReplayItems = 1;
        else g_cheats.allReplayItems = 0;
    }
    if (g_GameSettings.HasCheat(1)) {
        if (g_GameSettings.IsCheatActive(1)) g_cheats.infiniteAmmo = 1;
        else g_cheats.infiniteAmmo = 0;
    }
    if (g_GameSettings.HasCheat(2)) {
        if (g_GameSettings.IsCheatActive(2)) g_cheats.oneShotKillsEnemies = 1;
        else g_cheats.oneShotKillsEnemies = 0;
    }
    if (g_GameSettings.HasCheat(3)) {
        if (g_GameSettings.IsCheatActive(3)) g_cheats.unknown_14 = 1;
        else g_cheats.unknown_14 = 0;
    }
    if (g_GameSettings.HasCheat(4)) {
        if (g_GameSettings.IsCheatActive(4)) g_cheats.keepGrenadeSpeed = 1;
        else g_cheats.keepGrenadeSpeed = 0;
    }
    if (g_GameSettings.HasCheat(5)) {
        if (g_GameSettings.IsCheatActive(5)) g_cheats.unknown_18 = 1;
        else g_cheats.unknown_18 = 0;
    }
    if (g_GameSettings.HasCheat(6)) {
        if (g_GameSettings.IsCheatActive(6)) g_cheats.unknown_8 = 1;
        else g_cheats.unknown_8 = 0;
    }
    if (g_GameSettings.HasCheat(7)) {
        if (g_GameSettings.IsCheatActive(7)) g_cheats.headShotsOnly = 1;
        else g_cheats.headShotsOnly = 0;
    }
    if (g_GameSettings.HasCheat(8)) {
        if (g_GameSettings.IsCheatActive(8)) g_cheats.unknown_2c = 1;
        else g_cheats.unknown_2c = 0;
    }
    if (g_GameSettings.HasCheat(9)) {
        if (g_GameSettings.IsCheatActive(9)) g_cheats.oneHitKills = 1;
        else g_cheats.oneHitKills = 0;
    }
    if (g_GameSettings.HasCheat(10)) {
        if (g_GameSettings.IsCheatActive(10)) g_cheats.invincible = 1;
        else g_cheats.invincible = 0;
    }
    if (g_GameSettings.HasCheat(11)) {
        if (g_GameSettings.IsCheatActive(11)) g_cheats.unknown_c = 1;
        else g_cheats.unknown_c = 0;
    }
}
