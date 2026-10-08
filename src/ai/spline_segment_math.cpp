// AI-assisted reconstruction from GR8E69; see docs/Paths.md.
#include "AISplinePath.h"
static inline void ScaleInto(CVector3 &out, const CVector3 &v, float t) {
    out.x = v.x * t;
    out.y = v.y * t;
    out.z = v.z * t;
}
void CAISplinePathSegment::Set(float step, const CVector3 &a, const CVector3 &b, const CVector3 &c, const CVector3 &d) {
    parameterStep = step;
    cubic = a;
    quadratic = b;
    linear = c;
    constant = d;
}
void CAISplinePathSegment::Expand(float t, CVector3 &out) {
    ScaleInto(out, cubic, t);
    out += quadratic;
    out *= t;
    out += linear;
    out *= t;
    out += constant;
}
void CAISplinePathSegment::ExpandDerivative(float t, CVector3 &out) {
    ScaleInto(out, cubic, t * 1.5f);
    out += quadratic;
    out *= t * 2.0f;
    out += linear;
}
void CAISplinePathSegment::ExpandSecondDerivative(float t, CVector3 &out) {
    ScaleInto(out, cubic, t * 3.0f);
    out += quadratic;
    out *= 2.0f;
}
