// AI-assisted reconstruction from GR8E69. Fragments of the original bot object source: the
// original file name is not known. Method names come from the symbol table; member names
// are inferred, see docs/Behaviors.md.
#include "AIBot.h"

void CBotObject::ChooseWeapon() {
    if (CanFireWeapon()) {
        RateWeapons();
        if (bestWeaponIndex != currentWeaponIndex) {
            float current = (&weapons[currentWeaponIndex])->rating;
            float best = (&weapons[bestWeaponIndex])->rating;
            if (current <= 0.0f || !(best - current <= 0.1f))
                ChangeWeapon(bestWeaponIndex);
        }
    }
}
