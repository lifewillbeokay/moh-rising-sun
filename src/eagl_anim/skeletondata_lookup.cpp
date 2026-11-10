// AI-assisted reconstruction of SkeletonData's name lookups; see docs/Animation.md.
#include "profile.h"
#include "CharacterSkeleton.h"
#include <map>
#include <string>
unsigned int GetStringCRC(const char *);
typedef _STL::map<_STL::string, int> NameIndexMap;
namespace EALA {
namespace Character {
// The joint and attach-point maps are held as storage in the shared view.
static inline const NameIndexMap &JointMap(const SkeletonData *data) { return *(const NameIndexMap *)((const char *)data + 64); }
static inline const NameIndexMap &AttachMap(const SkeletonData *data) { return *(const NameIndexMap *)((const char *)data + 48); }
int SkeletonData::FindJoint(unsigned int crc) const {
    NameIndexMap::const_iterator it;
    for (it = JointMap(this).begin(); it != JointMap(this).end(); ++it) {
        if (GetStringCRC((*it).first.c_str()) == crc)
            return (*it).second;
    }
    return -1;
}
int SkeletonData::FindJoint(const char *name) const {
    NameIndexMap::const_iterator it = JointMap(this).find(_STL::string(name));
    if (it != JointMap(this).end())
        return (*it).second;
    return -1;
}
int SkeletonData::FindAttachPoint(const char *name) const {
    NameIndexMap::const_iterator it = AttachMap(this).find(_STL::string(name));
    if (it != AttachMap(this).end())
        return (*it).second;
    return -1;
}
}
}
