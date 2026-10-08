// AI-assisted reconstruction from GR8E69; see docs/Targeting.md.
#include "AITargeting.h"
float CAITargeting::GetDistanceToTargetSquaredXYZReal() {
    float distance;
    if (IsNonAITarget()) {
        distance = physics->position.DistanceSquared(modifiedPosition);
    } else {
        CAITarget current = GetTarget();
        distance = physics->position.DistanceSquared(current.aiObject->position);
    }
    return distance;
}
