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

int CBotMan::FindMMG(CBotObject *bot) {
    if (!mmgCount)
        return -1;
    float best = 9999999.0f;
    CVector3 position;
    int chosen = -1;
    ((ISceneNode *)bot)->GetPosition(position);
    for (unsigned int i = 0; i < mmgCount; i++) {
        if ((int)i != bot->mmgIndex && !g_BotMan.GetMMG(i)->IsInUse()) {
            CVector3 where;
            ((ISceneNode *)g_BotMan.GetMMG(i)->gun)->GetPosition(where);
            float distance = where.DistanceSquaredXY(position);
            if (!(distance >= best)) {
                best = distance;
                chosen = i;
            }
        }
    }
    return chosen;
}
