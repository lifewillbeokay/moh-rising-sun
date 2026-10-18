// AI-assisted scoped reconstruction from GR8E69; see docs/Script.md.
#ifndef GAME_WEAPON_H
#define GAME_WEAPON_H

// Member-only view of CWeapon for the reload built-ins. This does not establish
// the complete allocation, base classes or virtual interface.
// Observed fields of the weapon's property record; not a complete layout.
struct WeaponProperties {
    unsigned char unknown_0[4];
    short clipSize;   // +0x04; compared by BIFunc_PlayerWeaponInfo selector 4.
    short refireRate; // +0x06
    unsigned char unknown_8[6];
    short projectile; // +0x0e; nonzero for projectile weapons.
    unsigned char unknown_10[2];
    short infoValue1; // +0x12; returned for EWeaponInfo 1.
    unsigned char unknown_14[0x68 - 0x14];
    short weaponType; // +0x68; returned by the weapon-type built-ins.
};

// Original enum tag; its members are unknown.
enum EWeaponShootType { WeaponShootTypeUnknown = 0 };

// Original enum tag; selector names are descriptive.
enum EWeaponInfo { WeaponInfo0 = 0, WeaponInfo1 = 1, WeaponInfo2 = 2 };

class CWeapon {
  public:
    unsigned char unknown_0[0x300];
    WeaponProperties *properties; // +0x300
    unsigned char unknown_304[0x308 - 0x304];
    int crc;          // +0x308; weapon name CRC.
    short reserveAmmo; // +0x30c
    short infoValue0; // +0x30e; returned for EWeaponInfo 0.
    unsigned char unknown_310[0x320 - 0x310];
    int weaponClass; // +0x320; returned by BIFunc_PlayerGetWeaponClass.
    unsigned char unknown_324[0x350 - 0x324];
    unsigned int reloading : 1; // +0x350, bit 31: cleared by BIFunc_PlayerClearReloading.
    unsigned int unknown_350 : 31;
    void AddAmo(short);
    void StartReloading();
    void ReloadSingle();
    void DoneReloading();
    void Shoot(EWeaponShootType, float, float);
};

int GetWeaponInfoHelper(CWeapon *, EWeaponInfo);

#endif
