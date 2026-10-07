// AI-assisted reconstruction from GR8E69; see docs/Geometry.md.
#include "CLine2.h"
float CLine2::GetClosestPointSquared(const CVector2& point,CVector2* out) const {
    CVector2 direction=end;
    direction-=start;
    CVector2 delta=point;
    delta-=start;
    float dot=direction.Dot(delta);
    float squaredLength=direction.LengthSquared();
    CVector2 closest;
    if (!(dot<squaredLength)) closest=end;
    else if (!(dot>=0.0f)) closest=start;
    else {
        direction*=dot/squaredLength;
        closest=start;
        closest+=direction;
    }
    if(out) *out=closest;
    closest-=point;
    return closest.LengthSquared();
}
