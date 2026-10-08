// AI-assisted reconstruction from GR8E69; see docs/Paths.md.
#include "AIMovement.h"
void CAIPhysics::UpdateOrientationFrom(const CVector3 &a, const CVector3 &b, const CVector3 &c) {
    axes.axis0 = a;
    axes.axis1 = b;
    axes.axis2 = c;
    CalculateDirectionsFromCoord(eulerDirection, axes);
}
