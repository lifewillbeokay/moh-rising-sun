// AI-assisted reconstruction from GR8E69; see docs/Targeting.md.
#include "AITargeting.h"
void CAITargeting::GetLastSeenTargetPosition(CVector3 &out) {
    out = lastSeenPosition;
}
