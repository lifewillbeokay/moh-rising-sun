#ifndef GAME_FRUSTUM_H
#define GAME_FRUSTUM_H

// AI-assisted scoped runtime views from GR8E69; see docs/Camera.md.
// Field names are descriptive. These views establish the observed plane stride
// and frustum prefix, not complete historical classes or allocation contracts.
#include "CMatrix.h"
class CLine3;

struct CPlane {
    CVector3 normal;
    float distance;
    unsigned char unknown14[12];
    CPlane() {}
    ~CPlane() {}
    CPlane &operator=(const CPlane &other) {
        if (&other != this) {
            normal = other.normal;
            distance = other.distance;
        }
        return *this;
    }
    CPlane(const CVector3 &n, float d) : normal(n), distance(d) {}
    void Transform(const CMatrix &); // Original external body.
    int GetIntersection(const CLine3 &, float &, CVector3 *) const;
};

struct CFrustum {
    CPlane planes[10];
    void Transform(const CMatrix &);
};

typedef char CPlaneStrideCheck[sizeof(CPlane) == 0x20 ? 1 : -1];
typedef char CFrustumPrefixSizeCheck[sizeof(CFrustum) == 0x140 ? 1 : -1];

#endif
