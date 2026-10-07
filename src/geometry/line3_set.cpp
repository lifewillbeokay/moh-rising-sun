// AI-assisted reconstruction from GR8E69; see docs/Geometry.md.
#include "CLine3.h"
static inline void SubtractComponents(CVector3& out,const CVector3& a,const CVector3& b){
    out.x=a.x-b.x;
    out.y=a.y-b.y;
    out.z=a.z-b.z;
}
void CLine3::Set(const CVector3& a,const CVector3& b) {
    start=a;
    end=b;
    SubtractComponents(direction,end,start);
    lengthValid=0;
    lengthSquaredValid=0;
}
