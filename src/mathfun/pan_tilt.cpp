// AI-assisted reconstruction from GR8E69; see docs/MathFun.md.
#include "CVector3.h"
float MathFunAtan2F(float,float);
float MathFunNormalizeAngleNegativePiToPi(float);
void MathFunRotateAboutZ(float*,float*,float);
float MathFunGetPanAngleDiffNoRoll(CVector3* a,CVector3* b,CVector3* c) {
    b->x-=a->x;
    b->y-=a->y;
    b->z-=a->z;
    float angle=MathFunAtan2F(-c->y,c->x);
    return MathFunNormalizeAngleNegativePiToPi(MathFunAtan2F(b->x,b->y)-angle);
}
float MathFunGetTiltAngleDiffNoRoll(CVector3* a,CVector3* b,CVector3* c,CVector3* d) {
    float& by=b->y;
    float heading=MathFunAtan2F(-c->y,c->x);
    MathFunRotateAboutZ(&d->x,&d->y,-heading);
    b->x-=a->x;
    by-=a->y;
    b->z-=a->z;
    float angle=MathFunAtan2F(b->x,b->y);
    MathFunRotateAboutZ(&b->x,&by,-angle);
    float current=MathFunAtan2F(-d->z,d->y);
    return MathFunNormalizeAngleNegativePiToPi(MathFunAtan2F(-b->z,b->y)-current);
}
