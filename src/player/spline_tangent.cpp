// AI-assisted reconstruction from GR8E69; see docs/Paths.md.
#include "AISplinePath.h"
static void EvaluateSplineTangent(CAISplinePath *path, float parameter, CVector3 &out) {
    float scaled = parameter * path->lastSegment;
    int index = (int)scaled;
    path->segments[index].ExpandDerivative(scaled - index, out);
}
