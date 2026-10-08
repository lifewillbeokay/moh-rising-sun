// AI-assisted reconstruction from GR8E69; see docs/Paths.md.
#include "AISplinePath.h"
bool CAISplinePathBreakdownPoint::WasCrossedDistance(const CVector3 &point, float *previous) {
    float distance = position.DistanceSquaredXY(point);
    if (!(distance > 16.0f)) {
        if (distance <= 0.25f || !(distance <= *previous))
            return true;
    }
    *previous = distance;
    return false;
}
