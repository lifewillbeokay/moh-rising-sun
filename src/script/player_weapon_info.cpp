// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"
#include "CameraShake.h"
#include "Weapon.h"

void BIFunc_PlayerWeaponInfo(int **stack, void *object) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int crc = *(*stack - (count - 1));
    int info = *(*stack - (count - 2));
    CPlayerObject *player;
    if (object && ((ISceneNode *)object)->AsPlayerObject())
        player = ((ISceneNode *)object)->AsPlayerObject();
    else
        player = g_scene.GetPlayer(0);
    CPlayerWeaponObject *weaponObject = player->GetWeaponByCRC(crc);
    CWeapon *weapon;
    if (weaponObject)
        weapon = weaponObject->weapon;
    else {
        weapon = player->thrownWeapon;
        if (weapon && weapon->crc != crc)
            weapon = 0;
    }
    int result = -1;
    if (weapon) {
        switch (info) {
        case 0:
            result = weapon->reserveAmmo + weapon->infoValue0;
            break;
        case 1:
            result = weapon->properties->infoValue1;
            break;
        case 2:
            result = weaponObject && weaponObject->IsSelectable() ? 1 : 0;
            break;
        case 3:
            result = weapon->infoValue0;
            break;
        case 4:
            result = weapon->infoValue0 == weapon->properties->clipSize;
            break;
        }
    }
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = result;
}
