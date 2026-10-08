// AI-assisted reconstruction from GR8E69; see docs/Paths.md.
#include "AIMovement.h"
void CAILocomotion::SetupArbitraryPointUpdate(EMovePointType type, BS_STRUCT_Vector_struct *out, float distance) {
    movePointType = type;
    arrivalDistance = distance;
    scriptPosition = out;
}
