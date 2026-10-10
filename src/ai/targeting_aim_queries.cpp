// AI-assisted reconstruction from GR8E69; see docs/Targeting.md.
#include "AITargeting.h"
float MathFunRandomRealSigned(float, float);
void CAITargeting::GetAdjustedAimTargetPosition(CVector3 &out) {
    bool found = false;
    if (target.HasTarget()) {
        if (!GetTarget().aiObject) {
            if (lastSeenSet) {
                GetLastSeenTargetPosition(out);
                out.z += 1.0f;
                found = true;
            }
        } else {
            GetModifiedTargetPosition(out);
            found = true;
        }
    }
    if (!found) {
        const CVector3 &position = physics->position;
        const CVector3 &forward = physics->axes.axis1;
        out.x = position.x + forward.x;
        out.y = position.y + forward.y;
        out.z = position.z + forward.z;
        out.z += 1.0f;
    }
}

bool CAITargeting::DoBlindFire() {
    int percent = 0;
    switch (blindFireMode) {
    case 0: percent = 10; break;
    case 1: percent = 25; break;
    case 2: percent = 50; break;
    }
    return MathFunRandomRealSigned(0.0f, 100.0f) > percent;
}
