// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"
#include "SceneObjects.h"
#include "Weapon.h"

void BIFunc_BotDoneReloading(int **stack, void *object) {
    CSoldierObject *bot = (CSoldierObject *)((ISceneNode *)object)->AsBotObject();
    bot->weapons[bot->currentWeaponSlot]->DoneReloading();
    bot->reloadTimer = -1.0f;
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
