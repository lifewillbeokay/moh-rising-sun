// AI-assisted reconstruction from GR8E69; see docs/Paths.md.
#include "AIMovement.h"
#include "AIFilterGlobal.h"
#include "ScriptTimers.h"
void CAILocomotion::WalkToArbitraryPoint() {
    if (!(g_aigAIFilterGlobalObject.time >= updateTime)) {
        UpdateArbitraryPoint();
        objectivePosition = arbitraryPosition;
    } else {
        updateTime = g_aigAIFilterGlobalObject.time + 0.1f;
    }
    float distance;
    if (movePointType == MovePointType4)
        distance = physics->position.Distance(objectivePosition);
    else
        distance = physics->position.DistanceXY(objectivePosition);
    if (!(distance - arrivalDistance >= 0.5f))
        BSObjectTriggerEvent(object, 154, 0, 0, true);
}
