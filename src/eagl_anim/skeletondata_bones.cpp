// AI-assisted reconstruction of the skeleton data's bone count; see docs/Animation.md.
#include "EAGLAnim/Skeleton.h"
#include "CharacterSkeleton.h"
using namespace EALA::Character;
int SkeletonData::GetBoneCount() const {
    return skeleton->GetNumBones();
}
