// AI-assisted reconstruction of CAITargeting's construction; see docs/Behaviors.md. Member names are inferred.
#include "AITargeting.h"

CAITargeting::CAITargeting(CAIPhysics *physics, CAIObjectParameters *parameters, ISceneNode *sceneNode)
    : physics(physics), parameters(parameters), sceneNode(sceneNode), lastSeenTime(0.0f), lastSeenSet(0),
      visionVisible(0), unknownF0(0), alerted(0), targetUpdateTime(0.0f), weapon(0), aimMode(WeaponAimModeUnset),
      inverseBulletSpeed(1.0f) {}
