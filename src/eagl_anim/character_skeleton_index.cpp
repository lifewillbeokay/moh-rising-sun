// AI-assisted reconstruction of the character skeleton's joint index selection; see docs/Animation.md.
#include "CharacterSkeleton.h"
using namespace EALA::Character;
int Skeleton::GetJointIndex(TYPE_JOINT type) const {
    if (type == TYPE_JOINT_0)
        return 0;
    if (type == TYPE_JOINT_1)
        return 1;
    return -1;
}
