// AI-assisted reconstruction from GR8E69. A fragment of the original bot object source: the
// original file name is not known. Method names come from the symbol table.
#include <string.h>
#include "AIBot.h"

float CBotObject::GetClock() { return g_BotMan.clock; }
CBotObject *CBotObject::GetEnemyAI(unsigned int index) { return g_BotMan.enemies[index]; }
EnemyInfo &CBotObject::GetEnemyInfo(unsigned int index) { return enemyInfo[index]; }
EnemyInfo *CBotObject::GetpEnemyInfo(unsigned int index) { return &enemyInfo[index]; }

int CBotObject::EnemyInView() {
    return enemyInView ? enemyInView->inView != 0 : 0;
}

float CBotObject::GetHealth() { return aiObject->hitPoints; }
char *CBotObject::GetName() { return name; }
unsigned char CBotObject::GetPlayerIndex() { return playerIndex; }
unsigned char CBotObject::GetTeam() { return aiObject->team; }
float CBotObject::GetHealthPercent() { return aiObject->hitPoints * 100.0f / aiObject->maxHitPoints; }

void CBotObject::SetLookTarget(const CVector3 &target) {
    lookTargetExpireTime = GetClock() + 1.0f;
    lookAlongPath = 0;
    lookTarget = target;
    if (!(GetClock() <= lookTargetExpireTime))
        lookTargetExpireTime = 0.0f;
}

void CBotObject::SetMoveTarget(const CVector3 &target) { moveTarget = target; }
void CBotObject::LookAlongPath() { lookAlongPath = 1; }
int CBotObject::FollowingAStar() { return *(int *)((char *)aiObject + 492) == 2; }
void CBotObject::StopAStar() { *(int *)((char *)aiObject + 492) = 0; }

int CBotObject::WithinGrenadeBlastRadius() {
    int within = 0;
    if (!(grenadeDistanceSquared < 0.0f))
        within = grenadeDistanceSquared <= 36.0f;
    return within;
}

void *CBotObject::GetCurrentPathNode() { return *(void **)((char *)aiObject + 580); }
CampingPoint *CBotObject::GetCurrentCampingPoint() { return &g_BotMan.campingPoints[currentCampingPoint]; }
void CBotObject::SetPlayerIndex(unsigned char index) { playerIndex = index; }
void CBotObject::SetName(const char *newName) { strcpy(name, newName); }
