// AI-assisted reconstruction from GR8E69; see docs/Targeting.md.
#include "AITargeting.h"
bool CAITargeting::GetTargetGuessPosition(CVector3 &out) {
    if (lastSeenSet) {
        GetLastSeenTargetPosition(out);
        return true;
    }
    return false;
}
