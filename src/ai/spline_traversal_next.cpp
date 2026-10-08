// AI-assisted reconstruction from GR8E69; see docs/Paths.md.
#include "AISplinePath.h"
#include <math.h>
void CAISplinePathTraversal::MakeModifiedNextPoint() {
    float t = parameter + segment->parameterStep * 4.0f;
    if (!(t <= 1.0f)) {
        CVector3 direction;
        parameter = 1.0f;
        segment->Expand(1.0f, nextPoint.position);
        GetCurrentDerivative(&direction);
        float length = sqrtf(direction.LengthSquared());
        if (length != 0.0f) direction *= 1.0f / length;
        direction *= 0.25f;
        nextPoint.position += direction;
    } else {
        segment->Expand(t, nextPoint.position);
    }
}
