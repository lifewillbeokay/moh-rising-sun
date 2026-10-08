// AI-assisted reconstruction from GR8E69; see docs/Paths.md.
#include "AISplinePath.h"
void CAISplinePathTraversal::GetFinalPointForward(CVector3 *point) { path->GetLastPoint(point); }
void CAISplinePathTraversal::GetCurrentDerivative(CVector3 *point) { segment->ExpandDerivative(1.0f, *point); }
