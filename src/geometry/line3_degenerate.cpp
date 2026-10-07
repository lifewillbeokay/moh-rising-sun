// AI-assisted reconstruction from GR8E69; see docs/Geometry.md.
#include "CLine3.h"
int CLine3::IsDegenerate() const {
    return CachedLengthSquared()<1.0e-12f;
}
