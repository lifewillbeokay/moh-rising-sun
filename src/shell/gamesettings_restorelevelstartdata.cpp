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

void CGameSettings::RestoreLevelStartData() {
    unsigned int i;
    profile->saveData.savePointID = 0;
    profile->saveData.stats.Reset();
    profile->levelStats.Reset();
    for (i = 0; i <= 3; i++) SetCurrentCompartmentID(i, i);
    LevelFileContentsStruct_ *tuning = TLT_FindNextResourceByType((TLTResourceID)27, 0);
    unsigned int count = ((CompartmentListEntry *)tuning->data[101])->count;
    if (count > 1) {
        for (i = 0; i <= 3; i++) SetCurrentCompartmentID(i, 0);
        for (i = 0; i < count; i++) SetCurrentCompartmentID(i, ((CompartmentListEntry *)tuning->data[101])->ids[i]);
    }
    levelStartData = profile->saveData;
}
