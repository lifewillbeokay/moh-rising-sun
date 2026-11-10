// AI-assisted reconstruction from GR8E69; see docs/CameraShake.md.
#include "CameraShake.h"
extern "C" int sprintf(char *, const char *, ...);
float MathFunRandomReal(float, float);

CPlayerObject::CCameraShake::CCameraShake() : current(0.0f), target(0.0f), field_0c(0.0f), fadeTime(0.0f), duration(0.0f), elapsed(0.0f), rate(0.0f) {}

void CPlayerObject::CCameraShake::SetShake(float intensity, float rise, float fade, float seconds) {
    if (seconds < 0.0f || !(seconds <= 100000.0f)) {
        char message[256];
        sprintf(message, "Specified duration must be between 0 and %f -- %f specified\n", 100000.0, seconds);
        seconds = 0.0f;
    }
    intensity += intensity;
    if (!(seconds >= rise)) seconds = rise;
    target = intensity;
    if (!(rise >= 1.0f)) rise = 1.0f;
    if (!(fade >= 1.0f)) fadeTime = 1.0f;
    else fadeTime = fade;
    duration = seconds;
    elapsed = 0.0f;
    if (!(current <= 0.0f)) scale = 1.0f / current;
    else scale = 1.0f;
    rate = (target - current) / rise;
}

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

void CPlayerObject::CCameraShake::Update(float dt) {
    if (rate != 0.0f) {
        current += rate * dt;
        if ((!(rate >= 0.0f) && current < target) || (!(rate <= 0.0f) && !(current <= target))) {
            current = target;
            rate = 0.0f;
        }
    } else if (duration != 100000.0f && !(elapsed <= duration)) {
        SetShake(0.0f, fadeTime);
        return;
    }
    elapsed += dt;
}
