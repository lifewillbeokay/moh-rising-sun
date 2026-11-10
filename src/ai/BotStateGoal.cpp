// AI-assisted reconstruction from GR8E69; see docs/Behaviors.md. The original file name is not known.
#include "SceneObjects.h"
#include "AIBot.h"
#include "AISplinePath.h"
#include "MathFun.h"
long long MathFunRandomI64(long long, long long);

// Whether the walk has run off its node list with no pending step. Name inferred; inlined in its callers.
static inline int PathWalkEnded(CAIAreaPathFinding &path) {
    int ended = 0;
    if (path.pathFinished || (int)path.currentNode == path.field80)
        ended = 1;
    return ended;
}

int CBotObject::GoalReached() {
    if (!FollowingAStar())
        return 1;
    switch (objectiveType) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        if (PathWalkEnded(aiObject->areaPathFinding) && !aiObject->areaPathFinding.fieldA4 &&
            aiObject->areaPathFinding.walkPhase != 2) {
            CVector3 where(aiObject->position);
            if (!(aiObject->areaPathFinding.destination.DistanceSquaredXY(where) >= 0.25f))
                return 1;
        }
        break;
    case 8:
        if (PathWalkEnded(aiObject->areaPathFinding) && !aiObject->areaPathFinding.fieldA4 &&
            aiObject->areaPathFinding.walkPhase != 2) {
            CVector3 where(aiObject->position);
            if (!(aiObject->areaPathFinding.destination.DistanceSquaredXY(where) >= 9.0f))
                return 1;
        }
        break;
    }
    if (objectiveType == 7 && !followTarget->enemyInView)
        return 1;
    return 0;
}

// Decides whether the bot should leave the fight for a weapon, health or ammunition, and starts the walk there.
int CBotObject::WantToRetreat() {
    int forWeapons = RetreatForWeapons();
    if (forWeapons) {
        wantHealth = 1;
        wantWeapon = 0;
        wantAmmo = 0;
    } else {
        int forHealth = RetreatForHealth();
        if (forHealth) {
            wantHealth = forWeapons;
            wantWeapon = 1;
            wantAmmo = forWeapons;
        } else if (RetreatForAmmo()) {
            wantHealth = forHealth;
            wantAmmo = 1;
            wantWeapon = forHealth;
        }
    }
    if (wantHealth) {
        int missing[4];
        CVector3 where;
        int count = 0;
        for (int i = 0; i < 4; i++) {
            if (!weapons[i].weapon) {
                missing[count] = i;
                count++;
            }
        }
        // The original takes the random value as the weapon slot and never reads the list it built.
        if (FindWeapon(&where, ((CSoldierObject *)this)->GetWeapon(MathFunRandomI64(0, count)))) {
            objectiveType = 2;
            CalcAStarPath(where, 0);
            return 1;
        }
    } else if (wantWeapon) {
        CVector3 where;
        if (FindHealth(&where)) {
            objectiveType = 0;
            CalcAStarPath(where, 0);
            return 1;
        }
    } else if (wantAmmo) {
        CVector3 where;
        if (FindAmmo(&where)) {
            objectiveType = 1;
            CalcAStarPath(where, 0);
            return 1;
        }
    }
    return 0;
}
