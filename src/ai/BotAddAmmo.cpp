// AI-assisted reconstruction from GR8E69. Fragments of the original bot object source: the
// original file name is not known. Method names come from the symbol table; member names
// are inferred, see docs/Behaviors.md.
#include "SceneObjects.h"
#include "Weapon.h"
#include "AIBot.h"

void CBotObject::AddAmmo(CWeapon *weapon, int amount) {
    int total = weapon->reserveAmmo + amount;
    if (total > weapon->properties->infoValue1)
        amount = weapon->properties->infoValue1 - weapon->reserveAmmo;
    weapon->AddAmo(amount);
    wantAmmo = 0;
}
