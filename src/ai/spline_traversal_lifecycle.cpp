// AI-assisted reconstruction from GR8E69; see docs/Paths.md.
#include "AISplinePath.h"
CAISplinePathTraversal::CAISplinePathTraversal() : state(0), path(0) {}
CAISplinePathTraversal::~CAISplinePathTraversal() {}
float CAISplinePathTraversal::GetCurrentParameterValue(float t, const CVector3 &point) {
    return segment->GetClosestParameter(t, point);
}
