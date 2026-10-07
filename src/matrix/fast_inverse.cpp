// AI-assisted reconstruction from GR8E69; see docs/Matrix.md.
#include "CMatrix.h"
static inline float DotComponents(const CVector3& a, const CVector3& b) {
    return a.x*b.x + a.y*b.y + a.z*b.z;
}
void CMatrix::FastInverse(const CMatrix& in) {
    CVector3 right(*reinterpret_cast<const CVector3*>(&in.row[0]));
    CVector3 front(*reinterpret_cast<const CVector3*>(&in.row[1]));
    CVector3 up(*reinterpret_cast<const CVector3*>(&in.row[2]));
    CVector3 pos(*reinterpret_cast<const CVector3*>(&in.row[3]));
    row[0].x = in.row[0].x;
    row[0].y = in.row[1].x;
    row[0].z = in.row[2].x;
    row[0].w = 0.0f;
    row[1].x = in.row[0].y;
    row[1].y = in.row[1].y;
    row[1].z = in.row[2].y;
    row[1].w = 0.0f;
    row[2].x = in.row[0].z;
    row[2].y = in.row[1].z;
    row[2].z = in.row[2].z;
    row[2].w = 0.0f;
    row[3].x = -DotComponents(pos, right);
    row[3].y = -DotComponents(pos, front);
    row[3].z = -DotComponents(pos, up);
    row[3].w = 1.0f;
}
