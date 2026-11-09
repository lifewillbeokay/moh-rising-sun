// AI-assisted adaptation of CC0 Frontline source/research, checked against GR8E69.
// Provenance, target changes and validation: docs/research/collision-batch.md.

#include "CDBCollisionInternals.h"
#include "CDBGeom.h"
#include "CLine3.h"
#include "CollisionMath.h"

#ifdef DECOMP_COMPARE
EClsnState CCDBVolBox::TestCollision(const CLine3 &line, CCollision &, bool) const {
    float nearTime = -3.4028235e38f;
    float farTime = 3.4028235e38f;
    CVector3 offset;
    CollisionSub(offset, center, line.start);
    if (!boxline_testaxis(axisX, offset, line.direction, halfSize[0], nearTime, farTime))
        return (EClsnState)1;
    if (!boxline_testaxis(axisY, offset, line.direction, halfSize[1], nearTime, farTime))
        return (EClsnState)1;
    if (!boxline_testaxis(axisZ, offset, line.direction, halfSize[2], nearTime, farTime))
        return (EClsnState)1;
    if (nearTime > 1.0f || !(farTime >= 0.0f))
        return (EClsnState)1;
    return (EClsnState)2;
}
#endif
