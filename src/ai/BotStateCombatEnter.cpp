// AI-assisted reconstruction from GR8E69. Fragments of the original bot object source: the
// original file name is not known. Method names come from the symbol table; member names
// are inferred, see docs/Behaviors.md.
#include "AIBot.h"
#include "AISplinePath.h"

void CBotObject::BS_Enter_Combat() {
    combatMode = 8;
    aiObject->areaPathFinding.StopAStarPathWalk();
    currentCampingPoint = -1;
    mmgIndex = -1;
    pathStep = -1;
    enemyOutOfSightFrames = 0;
    combatTimer = -1.0f;
}
