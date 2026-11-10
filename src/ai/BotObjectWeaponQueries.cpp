// AI-assisted reconstruction from GR8E69. A fragment of the original bot object source: the
// original file name is not known. Method names come from the symbol table.
#include "SceneObjects.h"
#include "Weapon.h"
#include "AIBot.h"
#include "ScriptTimers.h"

int CBotObject::CanFireWeapon() {
    if (reloadEndTime >= 0.0f || fireDelay >= 0.0f || !weapons[currentWeaponIndex].weapon || fireBlockState == 5)
        return 0;
    return 1;
}

void CBotObject::StartReload() {
    ((CSoldierObject *)this)->GetCurrentWeapon()->StartReloading();
    reloadEndTime = GetClock() + 10.0f;
    BSObjectTriggerEvent(scriptObject, 361, 0, 0, false);
}

void CBotObject::FireAtEnemy() { fireAtEnemy = 1; }
void CBotObject::Jump() { jump = 1; }
void *CBotObject::GetGrenade() { return grenade; }
