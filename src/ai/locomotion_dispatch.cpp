// AI-assisted reconstruction from GR8E69; see docs/Paths.md.
#include "AIMovement.h"
void CAILocomotion::RegisterWalkType(IAILocomotionMethod *method) {
    methods[method->GetWalkType()] = method;
}
void CAILocomotion::StopWalkByType(AIOBJECT_PATH_WALK_TYPE type) {
    methods[type]->StopWalk();
}
