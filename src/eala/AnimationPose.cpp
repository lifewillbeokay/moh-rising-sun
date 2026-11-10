// AI-assisted reconstruction of source/Character/Animation.cpp; see docs/Animation.md.
#include "profile.h"
#include "CharacterAnimation.h"
namespace EALA {
namespace Character {

void Animation::GetRootTransform(float *pose, Skeleton *, COORD3 &translation, COORD4 &rotation) {
    translation.x = pose[8];
    translation.y = pose[9];
    translation.z = pose[10];
    rotation.x = pose[4];
    rotation.y = pose[5];
    rotation.z = pose[6];
    rotation.w = pose[7];
}

void Animation::SetRootTransform(float *pose, Skeleton *, const COORD3 &translation, const COORD4 &rotation) {
    pose[8] = translation.x;
    pose[9] = translation.y;
    pose[10] = translation.z;
    pose[4] = rotation.x;
    pose[5] = rotation.y;
    pose[6] = rotation.z;
    pose[7] = rotation.w;
}

void Animation::EvalSQT(Skeleton *, float time, float *pose, const EAGLAnim::BoneMask *mask) {
    float length;
    fnAnim->GetLength(length);
    float frame = min(time * ms_fPlaybackRate, length - 1.0f);
    fnAnim->EvalSQT(frame, pose, mask);
}

} // namespace Character
} // namespace EALA
