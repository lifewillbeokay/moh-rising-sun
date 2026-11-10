// AI-assisted reconstruction of the character skeleton's joint lookups; see docs/Animation.md.
#include "CharacterSkeleton.h"
using namespace EALA::Character;
int Skeleton::FindJoint(const char *name) const {
    if (HasData())
        return data->FindJoint(name);
    return -1;
}
int Skeleton::FindJoint(unsigned int crc) const {
    if (HasData())
        return data->FindJoint(crc);
    return -1;
}
bool Skeleton::IsJointBvExcluded(int joint) const {
    if (HasData())
        return data->IsJointBvExcluded(joint);
    return false;
}
void Skeleton::PrepForSkinning() {
    if (data)
        data->PrepForSkinning(matrices);
}
