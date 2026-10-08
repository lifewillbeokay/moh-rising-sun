// AI-assisted reconstruction from GR8E69; see docs/Paths.md.
#include "AIMovement.h"
#include <math.h>
static inline void SubtractInto(CVector3 &out, const CVector3 &a, const CVector3 &b) {
    out.x = a.x - b.x;
    out.y = a.y - b.y;
    out.z = a.z - b.z;
}
void CAIPhysics::UpdatePositionFrom(const CVector3 &point) {
    SubtractInto(displacement, point, position);
    position = point;
    movementLength = sqrtf(displacement.LengthSquared());
}
