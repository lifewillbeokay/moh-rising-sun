// AI-assisted reconstruction of the character skeleton's pose access; see docs/Animation.md.
#include "EAGLAnim/Skeleton.h"
#include "CharacterSkeleton.h"
using namespace EALA::Character;
void Skeleton::GetStillPose(float *pose) {
    data->skeleton->GetStillPose(pose, 0);
}
void Skeleton::StorePose(float *pose) {
    if (HasData() && data->skeleton)
        data->skeleton->PoseSQTToGlobal(pose, matrices, 0);
}
