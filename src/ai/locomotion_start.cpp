// AI-assisted reconstruction from GR8E69; see docs/Paths.md.
#include "AIMovement.h"
void CAILocomotion::StartWalkToArbitraryPoint(CVector3 &point) {
    methods[1]->StopWalk();
    methods[2]->StopWalk();
    walkType = 3;
    arbitraryPosition = point;
    objectivePosition = arbitraryPosition;
}
