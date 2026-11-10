// AI-assisted reconstruction of CAITargeting's position offset; see docs/Behaviors.md. Member names are inferred.
#include "AITargeting.h"

// The aim error range in the parameter block; the name is inferred.
static inline float AimErrorRange(CAIObjectParameters *parameters) { return ((float *)parameters)[3]; }
float MathFunRandomRealSigned(float, float);

void CAITargeting::UpdatePostionOffsetMin() {
    const float scale = 0.05f;
    CVector3 &offset = positionOffsetMin;
    float low = 0.25f;
    float high = 0.5f;
    positionOffsetMin.x = MathFunRandomRealSigned(AimErrorRange(parameters) * low, AimErrorRange(parameters) * high);
    positionOffsetMin.y = MathFunRandomRealSigned(AimErrorRange(parameters) * low, AimErrorRange(parameters) * high);
    positionOffsetMin.z = MathFunRandomRealSigned(AimErrorRange(parameters) * low, AimErrorRange(parameters) * high);
    offset *= scale;
}
