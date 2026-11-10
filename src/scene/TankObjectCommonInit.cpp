// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original Tank_object.cpp; member names are descriptive.
#include <string.h>
#include "SceneObjects.h"

void CTankObject::CommonInit() {
    memset(weaponSlots, -1, sizeof(weaponSlots));
    memset(weapons, 0, sizeof(weapons));
    lookObject = 0;
    pathNodeTimer = 0;
    flags.allowCollisionDamage = 1;
}
