// AI-assisted reconstruction from GR8E69; see docs/Paths.md.
#include "AIMovement.h"
#include "ScriptTimers.h"
void CAILocomotion::CantReachTarget() {
    BSObjectTriggerEvent(object, 194, 0, 0, true);
}
void CAILocomotion::SetArbitraryWalkMPType(EMovePointType type) {
    movePointType = type;
}
void CAILocomotion::TeleportTo(const CVector3 &position) {
    teleportPending = 1;
    teleportPosition = position;
}
