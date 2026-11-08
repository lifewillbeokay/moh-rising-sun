// AI-assisted partial reconstruction from GR8E69; see docs/SceneObjects.md.
// Frontline is an attributed semantic reference; target fields/control flow differ.
#include "SceneObjects.h"
#include "Weapon.h"

#if defined(DECOMP_COMPARE)
class CWeaponFactory {
  public:
    CWeapon *CreateWeaponByCRC(int, ISceneNode *, int);
};
extern CWeaponFactory g_WeaponFactory;

int CTankObject::AddWeapon(int slot, int crc) {
    if (crc == (int)0xfb9630c9) {
        weapons[slot] = 0;
        return 0;
    }
    weapons[slot] = g_WeaponFactory.CreateWeaponByCRC(crc, (ISceneNode *)this, 999);
    if (weapons[slot]) {
        weapons[slot]->bulletClass = weapons[slot]->properties->projectile ? 4 : 10;
        return 1;
    }
    return 0;
}
#endif
