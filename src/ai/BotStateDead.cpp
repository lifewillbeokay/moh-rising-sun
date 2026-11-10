// AI-assisted reconstruction from GR8E69. Fragments of the original bot object source: the
// original file name is not known. Method names come from the symbol table; member names
// are inferred, see docs/Behaviors.md.
#include "AIBot.h"

static float RESPAWN_DELAY = 6.0f;
static float DEATH_DELAY = 3.0f;

void CBotObject::BS_Enter_Dead() {
    SetMoveState((eBotMoveState)0);
    StopAStar();
    aiObject->SetAIStatusVar((aistatus_var)11, 1);
    respawnTime = GetClock() + RESPAWN_DELAY;
    wantAmmo = 0;
    wantWeapon = 0;
    wantHealth = 0;
    currentCampingPoint = -1;
    DropMPDumpObject();
}

int CBotObject::BS_Dead() {
    if (!(GetClock() <= respawnTime - RESPAWN_DELAY + DEATH_DELAY))
        field198 = -1000.0f;
    if (!(GetClock() <= respawnTime)) {
        if (Respawn()) {
            ClearBotData();
            ChangeState((eBotState)1, "ReSpawn");
        }
    }
    return 1;
}

void CBotObject::BS_Enter_Goto() {
    SetMoveState((eBotMoveState)2);
    SetNewObjectivePos();
    pathStep = -1;
}
