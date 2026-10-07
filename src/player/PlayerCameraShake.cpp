// AI-assisted reconstruction from GR8E69; see docs/CameraShake.md.
#include "CameraShake.h"
void CPlayerObject::SetCameraShake(float a, float b, float c, float d) { cameraShake.SetShake(a, b, c, d); }
void CPlayerObject::StartCameraShake(float a, float b) { cameraShake.SetShake(a, b); }
void CPlayerObject::StopCameraShake(float time) { cameraShake.SetShake(0.0f, time); }
void CPlayerObject::StartBackgroundCameraShake(float a, float b) { backgroundShake.SetShake(a, b); }
void CPlayerObject::StopBackgroundCameraShake(float time) { backgroundShake.SetShake(0.0f, time); }
void CPlayerObject::DoMotionShake(float amount, float rate) {
    motionShakeRate = rate;
    motionShakeAmount = amount;
    motionShakeTime = 0.0f;
    if (!(amount <= 0.0f))
        flags.motionShake = 1;
    else
        flags.motionShake = 0;
}
