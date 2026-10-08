// AI-assisted reconstruction from GR8E69; see docs/Paths.md.
#include "AISplinePath.h"
void CAISplinePathTraversal::SetupNextSegmentForward(const CVector3 &point) {
    float t = GetCurrentParameterValue(0.0f, point);
    while (t < 0.0f || t > 1.0f) {
        ++segmentIndex;
        ++segment;
        t = GetCurrentParameterValue(0.0f, point);
    }
    parameter = t;
    segment->Expand(t, nextPoint.position);
    MakeModifiedNextPoint();
}
void CAISplinePathTraversal::PrepareForTraversalForward(CAISplinePath *newPath, bool useAStar, CAIAreaPathFinding *finder) {
    path = newPath;
    segmentIndex = 0;
    segment = newPath->segments;
    parameter = 0.0f;
    segment->Expand(parameter, nextPoint.position);
    if (useAStar && finder->SetupAStarPathWalkForPatrol(nextPoint.position)) state = 2;
    else state = 1;
    previousDistanceSquared = 3.4028234663852886e38f;
}
void CAISplinePathTraversal::RestartForwardTraversal(CAIAreaPathFinding *finder) {
    PrepareForTraversalForward(path, true, finder);
}
