// AI-assisted reconstruction from GR8E69; see docs/Paths.md.
#include "AIMovement.h"
void CAILocomotion::StopWalkToArbitraryPoint() { walkType = 0; }
void CAILocomotion::ContinueWalkToArbitraryPoint() { walkType = 3; }
void CAILocomotion::GetCurrentObjectivePosition(CVector3 &out) { out = objectivePosition; }
CVector3 &CAILocomotion::GetCurrentObjectivePosition() { return objectivePosition; }
