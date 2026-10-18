// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "Weapon.h"

int GetWeaponInfoHelper(CWeapon *weapon, EWeaponInfo info) {
    switch (info) {
    case WeaponInfo0:
        return weapon->infoValue0;
    case WeaponInfo1:
        return weapon->properties->infoValue1;
    }
    return -1;
}
