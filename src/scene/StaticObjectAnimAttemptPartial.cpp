// AI-assisted partial reconstruction from GR8E69; see docs/SceneObjects.md.
// Frontline is an attributed semantic reference; target fields/control flow differ.
#include "SceneObjects.h"
#include "Volume.h"

#if defined(DECOMP_COMPARE)
void CStaticObject::AttemptAnimMotionUpdate(float dt) {
    if (motionLoop && motionRate != 0.0f) {
        CVector3 delta;
        delta.x = dt * velocity.x;
        delta.y = dt * velocity.y;
        delta.z = dt * velocity.z;
        motionStep = dt;
        float amount = dt * motionSampleRate;
        if (!(amount <= 1.0f))
            amount = 1.0f;
        else if (!(amount >= 0.0f))
            amount = 0.0f;
        CQuaternion rotation;
        rotation.Slerp(motionResetOrientation, animOrientation, amount);
        rotation.GetMatrix(transform);
        transform.SetPos(animPosition);
        transform.Translate(delta);
    }
    IVolume *world = (IVolume *)field_13c;
    if (world) {
        world->TransformedCopy(*worldVolume, transform);
        CVector3 min, max;
        ((IVolume *)field_13c)->GetExtents(min, max);
        sortMinX->value = min.x;
        sortMinY->value = min.y;
        sortMaxX->value = max.x;
        sortMaxY->value = max.y;
    } else {
        CVector3 position;
        ((ISceneNode *)this)->GetPosition(position);
        sortMinX->value = position.x;
        sortMinY->value = position.y;
        sortMaxX->value = position.x;
        sortMaxY->value = position.y;
    }
}
#endif
