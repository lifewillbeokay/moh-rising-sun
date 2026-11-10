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

void CGameSettings::InitControllerFromOptions(int player) {
    int index = GetProfileIndexFromPlayerID(player);
    int padOption = profile->profiles[index].unknown_28 != 0;
    g_inputMgr.padOption = padOption;
    if (profile->profiles[index].controllerSetting <= 2) {
        short buttons[14];
        SetUpButtonConfig(profile->profiles[index].controllerSetting, buttons);
        ButtonArrayToKeymap(buttons, g_keymap);
        ButtonArrayToKeymap(buttons, g_config_keymap[player]);
    } else {
        ButtonArrayToKeymap(profile->profiles[index].buttons[profile->profiles[index].controllerSetting - 3], g_keymap);
        ButtonArrayToKeymap(profile->profiles[index].buttons[profile->profiles[index].controllerSetting - 3], g_config_keymap[player]);
    }
    g_inputMgr.SetMasterBlockVibrations(!profile->profiles[index].noVibration, player);
}
