// AI-assisted reconstruction from GR8E69; see docs/Targeting.md.
#include "AITargeting.h"
void CAITargeting::SetLastSeenTargetPosition(const CVector3 &position) {
    lastSeenPosition = position;
    lastSeenSet = 1;
}
int CAITargeting::IsLastSeenTargetPositionSet() {
    return lastSeenSet;
}
int CAITargeting::CouldVisionSeeTarget() {
    return visionVisible;
}
