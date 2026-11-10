// AI-assisted reconstruction from GR8E69. Fragments of the original bot object source: the
// original file name is not known. Method names come from the symbol table; member names
// are inferred, see docs/Behaviors.md.
#include "SceneObjects.h"
#include "Weapon.h"
#include "AIBot.h"
#include "MPWeaponSet.h"

int CBotObject::RetreatForWeapons() {
    int i, count = 0;
    for (i = 0; i < 4; i++) {
        if (weapons[i].weapon)
            count++;
    }
    return count < (int)(3.5f - (character.skills[0] + character.skills[0]));
}

void CBotObject::DropMPDumpObject() {
    CWeapon *weapon = ((CSoldierObject *)this)->GetCurrentWeapon();
    if (weapon) {
        int ammo = weapon->reserveAmmo;
        ammo += weapon->loadedAmmo;
        CreateDumpObject(matrix, weapon->properties->crc, false, 0, ammo, (EPowerupType)3);
    }
}
