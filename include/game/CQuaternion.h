#ifndef GAME_CQUATERNION_H
#define GAME_CQUATERNION_H

// AI-assisted reconstruction; see docs/Geometry.md. Storage names are descriptive.
#include "CMatrix.h"

class CQuaternion {
public:
    float w, x, y, z;

    void Set(float x, float y, float z, float w);
    void Normalize();
    void GetMatrix(CMatrix&) const;
    void SetFromEuler(float, float, float);
    void Slerp(CQuaternion, CQuaternion, float);
};

typedef char CQuaternionStorageSizeCheck[sizeof(CQuaternion) == 16 ? 1 : -1];

#endif
