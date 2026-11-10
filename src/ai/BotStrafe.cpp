// AI-assisted reconstruction from GR8E69. A fragment of the original bot object source: the
// original file name is not known. Method names come from the symbol table.
#include <stdlib.h>
#include "AIBot.h"
#include "MathFun.h"

int CBotObject::WantToStrafe() {
    if (WithinGrenadeBlastRadius())
        return 1;
    float range = weapons[currentWeaponIndex].GetIdealRange(&character);
    return range < MathFunRandomReal(0.0f, 30.0f - character.skills[2] * 20.0f);
}
