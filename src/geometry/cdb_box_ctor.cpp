// AI-assisted adaptation of CC0 Frontline source/research, checked against GR8E69.
// Provenance, target changes and validation: docs/research/collision-batch.md.

#include "CDBGeom.h"

#ifdef DECOMP_COMPARE
static inline void CopyRow(CVector3 &out, const CDBVector &in) {
    out.x = in.x;
    out.y = in.y;
    out.z = in.z;
}
CCDBVolBox::CCDBVolBox(const CDBBox &box) {
    CopyRow(axisX, box.row[0]);
    CopyRow(axisY, box.row[1]);
    CopyRow(axisZ, box.row[2]);
    CopyRow(center, box.row[3]);
    halfSize[0] = box.halfSize.x * 0.5f;
    halfSize[1] = box.halfSize.y * 0.5f;
    halfSize[2] = box.halfSize.z * 0.5f;
}

#endif
