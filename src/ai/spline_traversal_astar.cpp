// AI-assisted reconstruction from GR8E69; see docs/Paths.md.
#include "AISplinePath.h"
void CAISplinePathTraversal::ContinueTraversalForwardAStar(CAIPhysics *physics, CAIAreaPathFinding *finder) {
    if (!(nextPoint.position.DistanceXY(physics->position) <= 1.5f)) {
        if (finder->SetupAStarPathWalkForPatrol(nextPoint.position)) state = 2;
        else state = 1;
        previousDistanceSquared = 3.4028234663852886e38f;
    } else if (state == 2) {
        state = 4;
    }
}
