#ifndef GAME_CHARACTER_SKELETON_H
#define GAME_CHARACTER_SKELETON_H
// AI-assisted scoped view of EALA::Character::Skeleton; see docs/Animation.md. The first word is the
// resource pointer the symbol Bind__...Pointer1Z...SkeletonData takes. Member names are inferred.
#include "EAGLTransform.h"
namespace EAGLAnim {
class Skeleton;
}
namespace EALA {
namespace Character {
class SkeletonData {
  public:
    unsigned char unknown_0[12];
    EAGLAnim::Skeleton *skeleton; // +12
    unsigned char unknown_10[80 - 16];
    int *excludedBegin; // +80; joints left out of the bounding volume, a vector's storage.
    int *excludedEnd;   // +84
    int GetBoneCount() const;
    int FindJoint(const char *) const;
    int FindJoint(unsigned int) const;
    bool IsJointBvExcluded(int);
    void PrepForSkinning(EAGL::Transform *);
};
class Skeleton {
  public:
    SkeletonData *data;   // +0
    unsigned char unknown_4[48];
    EAGL::Transform *matrices; // +52: 64-byte transforms, per FindAttachPoint.
    bool HasData() const { return data != 0; }
    int FindJoint(const char *) const;
    int FindJoint(unsigned int) const;
    bool IsJointBvExcluded(int) const;
    void PrepForSkinning();
    // Only the type name is recovered; the values are what GetJointIndex tests.
    enum TYPE_JOINT { TYPE_JOINT_0 = 0, TYPE_JOINT_1 = 1 };
    int GetJointIndex(TYPE_JOINT) const;
    void GetStillPose(float *);
    void StorePose(float *);
};
}
}
#endif
