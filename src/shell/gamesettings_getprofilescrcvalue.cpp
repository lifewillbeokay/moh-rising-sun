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

int CGameSettings::GetProfilesCRCValue() {
    CCRC *crc = new (DWI_alloc("source/system/crc.h:241", sizeof(CCRC), 1024)) CCRC;
    crc->width = 32;
    crc->polynomial = 0x04c11db7;
    crc->reflect = 1;
    crc->initial = 0xffffffff;
    crc->finalXor = 0xffffffff;
    crc->mask = 0xffffffff;
    crc->Reset();
    unsigned char *bytes = (unsigned char *)profile->profiles;
    for (int i = 0x31660; i != 0; i--) *crc += *bytes++;
    unsigned int result;
    if (crc->reflect) result = crc->finalXor ^ crc->u4Reflect(crc->value, crc->width);
    else result = crc->finalXor ^ crc->value;
    delete crc;
    return result;
}
