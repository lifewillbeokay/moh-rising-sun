// AI-assisted reconstruction from GR8E69; see docs/Geometry.md.
#include "CLine3.h"
static inline float DotComponents(const CVector3& a,const CVector3& b){
    return a.x*b.x+a.y*b.y+a.z*b.z;
}
static inline void ScaleComponents(CVector3& out,const CVector3& v,float s){
    out.x=v.x*s;
    out.y=v.y*s;
    out.z=v.z*s;
}
void CLine3::GetProjection(const CVector3& point,CVector3& out,float& t) const {
    if(!IsDegenerate()) {
        CVector3 delta;
        delta.Set(point.x-start.x,point.y-start.y,point.z-start.z);
        t=DotComponents(direction,delta)/CachedLengthSquared();
        ScaleComponents(out,direction,t);
        out+=start;
    }
    else {
        out=start;
        t=0.5f;
    }
}
