// AI-assisted reconstruction from GR8E69; see docs/Geometry.md.
#include "CQuaternion.h"
extern "C" float acosf(float),sinf(float);
void CQuaternion::Slerp(CQuaternion a,CQuaternion b,float t) {
    float ax=a.x,ay=a.y,az=a.z,aw=a.w;
    float dot=b.x*ax+b.y*ay+b.z*az+b.w*aw;
    if (!(dot>=0.0f)) {
        x=-ax;
        y=-ay;
        z=-az;
        w=-aw;
        dot=-dot;
    }
    else {
        x=ax;
        y=ay;
        z=az;
        w=aw;
    }
    float s0,s1;
    if (!(1.0f-dot<=0.0005f)) {
        float angle=acosf(dot);
        float denominator=sinf(angle);
        s0=sinf((1.0f-t)*angle)/denominator;
        s1=sinf(t*angle)/denominator;
    }
    else {
        s0=1.0f-t;
        s1=t;
    }
    float qx=s0*b.x+s1*x;
    float qy=s0*b.y+s1*y;
    float qz=s0*b.z+s1*z;
    float qw=s0*b.w+s1*w;
    x=qx;
    y=qy;
    z=qz;
    w=qw;
}
