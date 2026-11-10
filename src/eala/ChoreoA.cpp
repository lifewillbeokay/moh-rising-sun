// AI-assisted reconstruction of source/Character/Choreographer.cpp; see docs/Animation.md.
#include "profile.h"
#include "ResourceTypes.h"
namespace EALA {
namespace Character {

Choreographer::~Choreographer() {}

void Choreographer::AddPoseModifier(PoseModifier *modifier) {
    modifier->next = poseModifiers;
    poseModifiers = modifier;
}

} // namespace Character
} // namespace EALA
