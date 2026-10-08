// AI-assisted reconstruction from GR8E69; see docs/Paths.md.
#include "AIMovement.h"
extern float MathFunAtan2F(float, float);
static inline float ZComponent(const CVector3 &v) { return v.z; }
void CalculateDirectionsFromCoord(CAIFilterRealEulerDirection &direction, CAIFilterRealCoordinateAxes &axes) {
    CVector3 rotated;
    direction.angles.z = MathFunAtan2F(-axes.axis0.y, axes.axis0.x);
    rotated = axes.axis1;
    rotated.RotateAboutZ(-direction.angles.z);
    direction.angles.y = 0.0f;
    direction.angles.x = MathFunAtan2F(-ZComponent(rotated), rotated.y);
}
