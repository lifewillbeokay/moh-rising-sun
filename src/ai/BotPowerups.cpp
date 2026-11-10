// AI-assisted reconstruction from GR8E69; see docs/Behaviors.md. The original file name is not known.
#include "SceneObjects.h"
#include "AIBot.h"
#include "AISplinePath.h"
#include "MathFun.h"

int MathFunRandomSign();

// Every tenth call looks for a close health pack when hurt; the calls between consider a machine gun.
void CBotObject::CheckForNearbyPowerups() {
    static float MIN_MMG_SCORE = 5.0f;
    int countdown = --thinkCountdown;
    if (countdown <= 0) {
        thinkCountdown = 10;
        float health = GetHealthPercent();
        if (!(health >= 99.0f)) {
            CVector3 where;
            float distance = g_BotMan.FindClosestHealthPack(this, where);
            if (!(distance <= 0.0f)) {
                float range = (100.0f - health) * 0.25f + 10.0f;
                float limit = range * range;
                if (!(distance >= limit)) {
                    CalcAStarPath(where, 0);
                    if (!(aiObject->areaPathFinding.PathDistance() >= limit))
                        combatMode = 5;
                }
            }
        }
    } else {
        CVector3 where;
        if (!(g_BotMan.CalcMMGScore(this, countdown, where) <= MIN_MMG_SCORE) && !(character.skills[2] < 0.5f) &&
            MathFunRandomSign() > 0) {
            CalcAStarPath(where, 0);
            if (!(aiObject->areaPathFinding.PathDistance() >= 25.0f)) {
                combatMode = 6;
                mmgIndex = (unsigned int)thinkCountdown % g_BotMan.mmgCount;
            }
        }
    }
}
