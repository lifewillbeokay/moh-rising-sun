// AI-assisted scoped reconstruction from GR8E69; see docs/Script.md.
#ifndef GAME_MP_WEAPON_SET_H
#define GAME_MP_WEAPON_SET_H
class CMatrix;
struct BSObject;

// Original enum tag; member names are descriptive.
enum EPowerupType { PowerupTypeUnknown0 = 0, PowerupTypeHealth = 1, PowerupType2 = 2, PowerupType3 = 3 };
int CreateDumpObject(const CMatrix &, int, bool, BSObject *, int, EPowerupType);
// Scoped views of the multiplayer item lists; offsets are from the dump built-ins.
struct MPItemList {
    unsigned char unknown_0[8];
    int items[1]; // +0x8
};
struct MPWeaponSetData {
    unsigned char unknown_0[4];
    MPItemList *defaultAmmo; // +0x4
    MPItemList *weapons;     // +0x8
    unsigned char unknown_c[0x20 - 0xc];
    MPItemList *rankedAmmo;  // +0x20
    unsigned char unknown_24[0x28 - 0x24];
    MPItemList *health;      // +0x28
};
struct MPWeaponSetResource {
    unsigned char unknown_0[8];
    MPWeaponSetData *data; // +0x8
};
// Level resource found by BSInitBIFuncs (TLT resource type 13).
extern MPWeaponSetResource *g_pMPWeaponSet;

#endif
