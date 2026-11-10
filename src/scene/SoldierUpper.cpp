// AI-assisted reconstruction from GR8E69. Part of the original Soldier_object.cpp; there is no
// Frontline counterpart, so member names are descriptive and inferred from these accessors.
#include "SceneObjects.h"
#include "Weapon.h"

void CSoldierObject::SetUpperTranslation(const CVector3 &translation) { upperTranslation = translation; }

void CSoldierObject::SetUpperXAngleDifference(float a, float b) {
    upperXAngleMin = a;
    upperXAngleMax = b;
}

void CSoldierObject::SetUpperZAngleDifference(float a, float b) {
    upperZAngleMin = a;
    upperZAngleMax = b;
}

bool CSoldierObject::IsCurrentWeaponReloading() const {
    CWeapon *weapon = weapons[currentWeaponSlot];
    bool reloading = false;
    if (weapon)
        reloading = weapon->reloading;
    return reloading;
}

int CSoldierObject::GetCurrentWeaponType() const {
    CWeapon *weapon = GetCurrentWeapon();
    if (!weapon)
        return -1;
    return weapon->weaponClass;
}

CWeapon *CSoldierObject::GetCurrentWeapon() const { return weapons[currentWeaponSlot]; }

CWeapon *CSoldierObject::GetWeapon(int slot) const { return weapons[slot]; }
