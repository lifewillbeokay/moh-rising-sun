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

void CGameSettings::ButtonArrayToKeymap(short *buttons, KEYMAP *keymap) {
    BindKeyToEvent(keymap, 6, buttons[0]);
    BindKeyToEvent(keymap, 7, buttons[3]);
    BindKeyToEvent(keymap, 8, buttons[2]);
    BindKeyToEvent(keymap, 9, buttons[1]);
    BindKeyToEvent(keymap, 10, buttons[4]);
    BindKeyToEvent(keymap, 11, buttons[5]);
    BindKeyToEvent(keymap, 12, buttons[11]);
    BindKeyToEvent(keymap, 13, buttons[7]);
    BindKeyToEvent(keymap, 14, buttons[8]);
    BindKeyToEvent(keymap, 15, buttons[6]);
    BindKeyToEvent(keymap, 18, buttons[9]);
    BindKeyToEvent(keymap, 22, buttons[13]);
    BindKeyToEvent(keymap, 20, buttons[12]);
    BindKeyToEvent(keymap, 19, buttons[10]);
}
