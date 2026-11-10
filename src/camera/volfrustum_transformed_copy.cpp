// AI-assisted reconstruction of the frustum volume's transformed copy; see docs/Rendering.md.
#include "VolFrustum.h"
void DebugMsg(const char *, ...);
void CVolFrustum::TransformedCopy(const IVolume &other, const CMatrix &matrix) {
    CVolFrustum source = (const CVolFrustum &)other;
    for (int i = 0; i < 6; i++) {
        planes[i] = source.planes[i];
        planes[i].Transform(matrix);
    }
}
EClsnState CVolFrustum::TestCollision(const IVolume &, CCollision &, bool) const {
    DebugMsg("Undefined: CVolFrustum::TestCollision(const IVolume& volume, CCollision& context, bool findContacts)\n");
    return (EClsnState)0;
}
