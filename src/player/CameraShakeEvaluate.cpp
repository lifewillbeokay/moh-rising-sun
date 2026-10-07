// AI-assisted reconstruction from GR8E69; see docs/CameraShake.md.
#include "CameraShake.h"
float MathFunRandomReal(float, float);

void CPlayerObject::CCameraShake::SetShake(float intensity, float rampTime) { SetShake(intensity, rampTime, 0.0f, 100000.0f); }
void CPlayerObject::CCameraShake::Evaluate(float &x, float &y) {
    float amplitude = current * 0.25f;
    y += MathFunRandomReal(-amplitude, amplitude);
    x += MathFunRandomReal(-amplitude, amplitude);
    if (!(rate >= 0.0f)) {
        float s = scale * current;
        y *= s;
        x *= s;
    }
}
