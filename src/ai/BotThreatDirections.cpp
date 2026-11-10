// AI-assisted reconstruction from GR8E69. Fragments of the original bot object source: the
// original file name is not known. Method names come from the symbol table; member names
// are inferred, see docs/Behaviors.md.
#include "AIBot.h"
#include "AISplinePath.h"

int FindThreatPoints2(int, float, CBotObject *);

int CBotObject::UpdateThreatDirections() {
    CAreaPathNode *node = (CAreaPathNode *)GetCurrentPathNode();
    if (!node)
        return 0;
    return FindThreatPoints2(AIPathFinding::GetIndexByPathNode(node), 40.0f, this);
}

int CBotObject::BoredOfWaiting() {
    float clock = GetClock();
    float waited;
    if (!enemyInView || !(enemyInView->lastSeenTime >= campStartTime))
        waited = clock - campStartTime;
    else
        waited = clock - enemyInView->lastSeenTime;
    return waited > boredDelay;
}
