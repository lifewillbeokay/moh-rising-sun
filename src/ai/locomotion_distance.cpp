// AI-assisted reconstruction from GR8E69; see docs/Paths.md.
#include "AIMovement.h"
float CAILocomotion::GetDistanceToArbitraryPoint() {
    return physics->position.DistanceXY(arbitraryPosition);
}
