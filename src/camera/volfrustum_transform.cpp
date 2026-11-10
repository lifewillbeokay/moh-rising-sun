// AI-assisted reconstruction of the frustum's transform; see docs/Rendering.md.
#include "VolFrustum.h"
#include "CMatrix.h"
static inline void TransformPoint(CVector3 &out, const CMatrix &m, const CVector3 &p) {
    CVector3 r;
    r.x = p.x * m.row[0].x + p.y * m.row[1].x + p.z * m.row[2].x + m.row[3].x;
    r.y = p.x * m.row[0].y + p.y * m.row[1].y + p.z * m.row[2].y + m.row[3].y;
    r.z = p.x * m.row[0].z + p.y * m.row[1].z + p.z * m.row[2].z + m.row[3].z;
    out = r;
}
void CVolFrustum::Transform(const CMatrix &m) {
    for (int i = 0; i < 6; i++)
        planes[i].Transform(m);
    {
        CVector3 origin(0.0f, 0.0f, 0.0f);
        TransformPoint(point16, m, origin);
    }
    {
        CVector3 ahead(0.0f, 1.0f, 0.0f);
        TransformPoint(point32, m, ahead);
    }
    TransformPoint(boundCenter, m, point48);
}
