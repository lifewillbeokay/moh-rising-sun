// AI-assisted reconstruction from GR8E69. Fragments of the original AIActivityZone.cpp;
// method names come from the symbol table, member names are inferred.
#include "SceneNode.h"
#include "AIBehavior.h"

int CAIActivityZone::IsPointUsable(const CVector3 &position) {
    float distance;
    switch (mode) {
    case 0:
    default:
        return 1;
    case 1: {
        CVector3 target;
        CVector3 *where = &target;
        node->GetPosition(*where);
        distance = position.Distance(*where);
        break;
    }
    case 2:
        distance = position.Distance(point);
        break;
    }
    return distance < radius;
}

void CAIActivityZone::UseMovingTarget(ISceneNode *target, float newRadius) {
    node = target;
    radius = newRadius;
    mode = 1;
}

void CAIActivityZone::Disable() {
    node = 0;
    mode = 0;
}
