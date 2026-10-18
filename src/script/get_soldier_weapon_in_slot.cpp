// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "SceneObjects.h"

// A negative slot selects the current weapon of the running script's soldier.
CWeapon *GetSoldierWeaponInSlot(int slot) {
    CSoldierObject *soldier = (CSoldierObject *)g_pBSObject->gameObject->GetSceneNode();
    return slot < 0 ? soldier->weapons[soldier->currentWeaponSlot] : soldier->weapons[slot];
}
