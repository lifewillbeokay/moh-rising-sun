// AI-assisted partial reconstruction from GR8E69.
// See docs/research/explosion-grenade-updates.md.
#include "Bullet.h"
#include "CLine3.h"
#include "WorldObject.h"

#if defined(DECOMP_COMPARE)
// Descriptive helpers; the temporary forms are reconstruction choices.
static inline void TransformPoint(CVector3 &out, const CMatrix &m, const CVector3 &p) {
    CVector3 r;
    r.x = p.x * m.row[0].x + p.y * m.row[1].x + p.z * m.row[2].x + m.row[3].x;
    r.y = p.x * m.row[0].y + p.y * m.row[1].y + p.z * m.row[2].y + m.row[3].y;
    r.z = p.x * m.row[0].z + p.y * m.row[1].z + p.z * m.row[2].z + m.row[3].z;
    out = r;
}

static inline void SetTransformed(CVector3 &r, const CMatrix &m, const CVector3 &p) {
    r.x = p.x * m.row[0].x + p.y * m.row[1].x + p.z * m.row[2].x + m.row[3].x;
    r.y = p.x * m.row[0].y + p.y * m.row[1].y + p.z * m.row[2].y + m.row[3].y;
    r.z = p.x * m.row[0].z + p.y * m.row[1].z + p.z * m.row[2].z + m.row[3].z;
}

bool CExplosion::TestRay(const CVector3 &position, const CVector3 &startOffset,
                         const CVector3 &endOffset, const CMatrix &matrix) {
    CVector3 center = sphere.GetCenter();
    CVector3 end, start;
    TransformPoint(start, matrix, startOffset);
    SetTransformed(end, matrix, endOffset);
    end += position;
    start += center;
    CLine3 line(start, end);
    CCollision collision;
    collision.lineTest = 0;
    const IVolume *volume = (const IVolume *)((ISceneNode *)g_scene.world)->GetWorldBoundingVolume(
        (ISceneNode::EVolumeType)0);
    return volume->TestCollision(line, collision, false) != 1;
}
#endif
