// AI-assisted reconstruction of the skeleton data's bounding-volume exclusion test; see docs/Animation.md.
#include "CharacterSkeleton.h"
using namespace EALA::Character;
bool SkeletonData::IsJointBvExcluded(int joint) {
    for (int i = 0; i < excludedEnd - excludedBegin; i++) {
        if (joint == excludedBegin[i])
            return true;
    }
    return false;
}
