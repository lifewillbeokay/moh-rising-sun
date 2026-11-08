// AI-assisted partial reconstruction from GR8E69; see docs/SceneObjects.md.
// Frontline is an attributed semantic reference; target fields/control flow differ.
#include "SceneObjects.h"

#if defined(DECOMP_COMPARE)
void CStaticObject::Init() {
    orientation.SetFromMatrix(transform);
    scriptScaleBase = 1.0f;
    flags.drawEnabled = 1;
    flags.scriptMoving = 0;
    flags.scriptRotating = 0;
    flags.scriptScaling = 0;
    velocity.x = velocity.y = velocity.z = 0.0f;
    maxLightPriority = -1;
    pathState = 0;
    flags.allowCollisionDamage = 0;
    pathScale = 0.5f;
    flags.pathFlag1 = 0;
}
#endif
