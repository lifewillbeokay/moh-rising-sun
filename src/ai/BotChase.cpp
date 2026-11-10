// AI-assisted reconstruction from GR8E69. A fragment of the original bot object source: the original file name is not
// known. WantToStrafe and the three chase methods share a unit because their literal pool holds a double that must sit
// on an eight-byte boundary. Method names come from the symbol table; member names are inferred.
#include <stdlib.h>
#include "AIBot.h"
#include "MathFun.h"
#include "AISplinePath.h"
#include "SceneNode.h"
int FindCover2(int, const CVector3 &, CBotObject *, sCoverData &);

int CBotObject::WantToStrafe() {
    if (WithinGrenadeBlastRadius())
        return 1;
    float range = weapons[currentWeaponIndex].GetIdealRange(&character);
    return range < MathFunRandomReal(0.0f, 30.0f - character.skills[2] * 20.0f);
}

int CBotObject::ChaseDecision(bool orGoto) {
    float sinceSeen = GetClock() - enemyInView->lastSeenTime;
    float healthLead = GetHealth() - enemyInView->ai->hitPoints;
    CVector3 position;
    if (rand() & 3)
        position = EstimateEnemyPosition();
    else
        position = enemyInView->position;
    if (DirectChase(position)) {
        enemyInView->position = position;
        ChangeState((eBotState)4, "Direct Chase");
        return 1;
    }
    if (CoverPointChase(position, healthLead, sinceSeen)) {
        enemyInView->position = position;
        CAreaPathNode *node = AIPathFinding::GetPathNodeByIndex(cover.pathNode);
        CalcAStarPath(cover.positions[cover.index], node);
        coverPending = 1;
        combatMode = 1;
        coverTimers[0] = 0.0f;
        coverTimers[1] = 0.0f;
        coverTimers[2] = 0.0f;
        return 1;
    }
    if (orGoto) {
        ChangeState((eBotState)1, "Chase Decision :: Goto");
        return 1;
    }
    return 0;
}

int CBotObject::DirectChase(CVector3 &position) {
    float chance = character.skills[0] * 192.0f + 16.0f;
    float distance = character.skills[0] * 64.0f + 32.0f;
    if (combatMode == 4 || !((float)(rand() & 0xff) >= chance)) {
        float limit = MathFunRandomReal(0.0f, distance);
        if (!(enemyInView->distance >= limit)) {
            CalcAStarPath(position, 0);
            if (aiObject->areaPathFinding.PathDistance() < limit)
                return 1;
        }
    }
    return 0;
}

int CBotObject::CoverPointChase(CVector3 &position, float healthLead, float sinceSeen) {
    void *node = GetCurrentPathNode();
    if (node && FindCover2(AIPathFinding::GetIndexByPathNode((CAreaPathNode *)node), position, this, cover)) {
        CVector3 here;
        ((ISceneNode *)this)->GetPosition(here);
        float distance = CVector3(cover.positions[cover.index]).DistanceXY(here);
        float limit = MathFunRandomReal(character.skills[0] * 20.0f + 10.0f, character.skills[0] * 35.0f + 25.0f);
        distance -= healthLead * 0.2f;
        if (sinceSeen * 0.5f + distance < limit)
            return 1;
    }
    return 0;
}
