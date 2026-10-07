// AI-assisted reconstruction from GR8E69; see docs/Geometry.md.
#include "CQuaternion.h"
extern "C" float sqrtf(float);
void CQuaternion::Set(float ax,float ay,float az,float aw) {
    x=ax;
    y=ay;
    z=az;
    w=aw;
}
void CQuaternion::Normalize() {
    float s=1.0f/sqrtf(x*x+y*y+z*z+w*w);
    x*=s;
    y*=s;
    z*=s;
    w*=s;
}
