// AI-assisted reconstruction of CBotMan's camping point, health pack and machine gun searches; see docs/Behaviors.md.
// Method names come from the symbol table; member names are inferred.
#include "AIBot.h"
#include "SceneNode.h"
#include "MathFun.h"
#include "ObjCreate.h"
#include <stdlib.h>
extern int g_bTeamDM;
extern int g_NumBotsCreated;
extern CDumpObjectContainer g_DumpObjectContainer;

int CBotMan::FindCampingPoint(CBotObject *bot) {
    float best = 9999999.0f;
    CVector3 position;
    int chosen = -1;
    int lastGroup = -1;
    ((ISceneNode *)bot)->GetPosition(position);
    if (bot->currentCampingPoint >= 0)
        lastGroup = campingPoints[bot->currentCampingPoint].group;
    bool random = (rand() & 255) <= 127;
    for (unsigned int i = 0; i < campingPointCount; i++) {
        CampingPoint &point = campingPoints[i];
        if (point.group != lastGroup && point.lookatCount && !CampingPointInUseByTeammate(bot, i)) {
            float score = random ? MathFunRandomReal(0.0f, 100.0f) : position.DistanceSquared(point.position);
            if (!(score >= best)) {
                best = score;
                chosen = i;
            }
        }
    }
    return chosen;
}

int CBotMan::CampingPointInUseByTeammate(CBotObject *bot, int index) {
    if (g_bTeamDM) {
        CVector3 position;
        ((ISceneNode *)bot)->GetPosition(position);
        if (!(campingPoints[index].position.DistanceSquaredXY(position) >= 100.0f)) {
            int team = ((IMovingSceneNode *)bot)->GetTeam();
            for (int i = 0; i < g_NumBotsCreated; i++) {
                CBotObject *other = bots[i];
                if (other != bot && ((IMovingSceneNode *)other)->GetTeam() == team && other->fireBlockState == 6 &&
                    other->currentCampingPoint == index)
                    return 1;
            }
        }
    }
    return 0;
}

float CBotMan::FindClosestHealthPack(CBotObject *bot, CVector3 &found) {
    float best = 1.0e9f;
    CVector3 position;
    int any = 0;
    ((ISceneNode *)bot)->GetPosition(position);
    for (DumpObject *object = g_DumpObjectContainer.GetFirstDumpObject(); object;
         object = g_DumpObjectContainer.GetNextDumpObject(object)) {
        if (object->pickup.value20 == 1) {
            CVector3 where;
            ((ISceneNode *)object->object)->GetPosition(where);
            float distance = where.DistanceSquaredXY(position);
            if (!(distance >= best)) {
                best = distance;
                any = 1;
                found = where;
            }
        }
    }
    float result;
    if (!any)
        result = -1.0f;
    else
        result = best;
    return result;
}

float CBotMan::CalcMMGScore(CBotObject *bot, unsigned long index, CVector3 &gunPosition) {
    static float MAX_MMG_DIST = 25.0f;
    float score = -1.0f;
    if (mmgCount) {
        MMG_Info *mmg = &mmgs[index % mmgCount];
        if (!mmg->IsInUse()) {
            CVector3 position;
            gunPosition = mmg->position;
            ((ISceneNode *)bot)->GetPosition(position);
            float distance = position.DistanceXY(gunPosition);
            if (!(distance >= MAX_MMG_DIST) && bot->enemyInView) {
                CVector3 enemy(bot->enemyInView->position);
                CVector3 toEnemy(enemy);
                toEnemy -= gunPosition;
                float first = mmg->firstAxis.x * toEnemy.x + mmg->firstAxis.y * toEnemy.y;
                float second = mmg->secondAxis.x * toEnemy.x + mmg->secondAxis.y * toEnemy.y;
                if (!(first <= 0.0f) && !(second <= 0.0f)) {
                    float least = second;
                    if (!(least <= first))
                        least = first;
                    score = least - distance;
                }
            }
        }
    }
    return score;
}
