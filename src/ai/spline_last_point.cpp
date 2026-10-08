// AI-assisted reconstruction from GR8E69; see docs/Paths.md.
#include "AISplinePath.h"
void CAISplinePath::GetLastPoint(CVector3 *out) { segments[lastSegment].Expand(0.0f, *out); }
