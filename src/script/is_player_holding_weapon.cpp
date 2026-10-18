// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "SceneNode.h"
#include "CameraShake.h"
#include "Weapon.h"

// Weapon class 20 and an empty hand both count as not holding a weapon.
int IsPlayerHoldingWeapon() {
    CWeapon *weapon = g_scene.GetPlayer(0)->GetCurrentWeapon();
    int weaponClass = -1;
    if (weapon)
        weaponClass = weapon->weaponClass;
    if (weaponClass == 20 || weaponClass == -1)
        return 0;
    return 1;
}
