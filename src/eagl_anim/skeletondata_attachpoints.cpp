// AI-assisted reconstruction of how SkeletonData gathers its attach points; see docs/Animation.md.
#include "profile.h"
#include "CharacterSkeleton.h"
#include <string>
namespace EALA {
namespace Character {
// Attach points this game's models use under other joint names.
void SkeletonData::GatherAttachPoints() {
    RemapAttachPoint(_STL::string("head"), _STL::string("JO_helmet"));
    RemapAttachPoint(_STL::string("VisionCone"), _STL::string("JO_eye_L"));
    RemapAttachPoint(_STL::string("lt_hand"), _STL::string("JO_prop"));
    RemapAttachPoint(_STL::string("rt_hand"), _STL::string("JO_gun"));
}
}
}
