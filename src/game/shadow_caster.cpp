// AI-assisted reconstruction of the character shadow's caster; see docs/Rendering.md.
#include "CharacterSkeleton.h"
#include "CharacterShadow.h"
// A CAnimObject's skeleton lies at +0x88; the view here needs only that.
static inline EALA::Character::Skeleton *SkeletonOf(CAnimObject *object) {
    return (EALA::Character::Skeleton *)((char *)object + 0x88);
}
void CCharacterShadow::SetShadowCaster(CAnimObject *object) {
    caster = object;
    hipJoints[0] = SkeletonOf(caster)->FindJoint("JO_hip_L");
    hipJoints[1] = SkeletonOf(caster)->FindJoint("JO_hip_R");
    InitProjector();
}
