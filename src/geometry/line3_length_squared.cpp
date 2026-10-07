// AI-assisted reconstruction from GR8E69; see docs/Geometry.md.
#include "CLine3.h"
float CLine3::GetLengthSquared() const {
    return CachedLengthSquared();
}
