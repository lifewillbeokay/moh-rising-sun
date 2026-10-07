// AI-assisted reconstruction from GR8E69; see docs/Matrix.md.
#include "CMatrix.h"
extern "C" float sqrtf(float);
static inline float DotComponents(const CVector3& a,const CVector3& b) {
    return a.x*b.x+a.y*b.y+a.z*b.z;
}
static inline void NormalizeVector(CVector3& v) {
    float length=sqrtf(v.x*v.x+v.y*v.y+v.z*v.z);
    if(length!=0.0f){
        float s=1.0f/length;
        v.x*=s;
        v.y*=s;
        v.z*=s;
    }
}
static inline void ScaleVector(CVector3& out,const CVector3& v,float s){
    out.x=v.x*s;
    out.y=v.y*s;
    out.z=v.z*s;
}
static inline void SubtractVector(CVector3& out,const CVector3& a,const CVector3& b){
    out.x=a.x-b.x;
    out.y=a.y-b.y;
    out.z=a.z-b.z;
}
void CMatrix::Orthonormalize() {
    CVector3 projection;
    CVector3 oldRight(*reinterpret_cast<const CVector3*>(&row[0]));
    CVector3 oldFront(*reinterpret_cast<const CVector3*>(&row[1]));
    CVector3 oldUp(*reinterpret_cast<const CVector3*>(&row[2]));
    CVector3 right(oldRight);
    NormalizeVector(right);
    CVector3 front;
    float dot=DotComponents(oldFront,right);
    ScaleVector(projection,right,dot);
    SubtractVector(front,oldFront,projection);
    NormalizeVector(front);
    CVector3 up;
    dot=DotComponents(oldUp,right);
    ScaleVector(projection,right,dot);
    SubtractVector(up,oldUp,projection);
    dot=DotComponents(oldUp,front);
    ScaleVector(projection,front,dot);
    SubtractVector(up,up,projection);
    NormalizeVector(up);
    row[0].x=right.x;
    row[0].y=right.y;
    row[0].z=right.z;
    row[1].x=front.x;
    row[1].y=front.y;
    row[1].z=front.z;
    row[2].x=up.x;
    row[2].y=up.y;
    row[2].z=up.z;
}
