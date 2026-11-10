// AI-assisted reconstruction from GR8E69. Part of the original Soldier_object.cpp; there is no
// Frontline counterpart. Constrain follows CPlayerObject::Constrain's limit on a tick's movement.
#include "SceneObjects.h"
#include "Box.h"
extern "C" float sqrtf(float);
extern float LARGEST_MOVE_PER_TICK;

class CVolSphere {
  public:
    void TransformedCopy(const IVolume &, const CMatrix &);
};
class CClsnConstraintsBase {
  public:
    int SatisfyConstraints(CMatrix &, float, float, float);
};

static inline void SetSub(CVector3 &r, const CVector3 &a, const CVector3 &b) {
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    r.z = a.z - b.z;
}

void CSoldierObject::UpdateCollisionVolumes() {
    CMatrix &transform = ((CAnimObject *)this)->transform;
    ((CVolSphere *)worldVolumeType1)->TransformedCopy(*(IVolume *)localVolumeType1, transform);
    ((CVolSphere *)worldVolumeType2)->TransformedCopy(*(IVolume *)localVolumeType2, transform);
}

int CSoldierObject::Constrain(float) {
    CAnimObject *self = (CAnimObject *)this;
    if (collidedThisFrame || self->attached)
        return 1;
    CVector3 start = self->transform.GetPos();
    int satisfied = ((CClsnConstraintsBase *)&clsnConstraints)->SatisfyConstraints(self->transform, 1.0f, 1.0f, 1.0f);
    CVector3 moved;
    CVector3 end = self->transform.GetPos();
    SetSub(moved, end, start);
    moved *= 1.0f / self->animStep;
    float speed = sqrtf(moved.x * moved.x + moved.y * moved.y + moved.z * moved.z);
    if (!(speed <= LARGEST_MOVE_PER_TICK)) {
        float s = LARGEST_MOVE_PER_TICK / speed;
        moved *= s;
        moved *= self->animStep;
        start += moved;
        self->transform.SetPos(start);
    }
    return satisfied;
}
