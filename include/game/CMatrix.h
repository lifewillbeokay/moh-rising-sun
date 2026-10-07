#ifndef GAME_CMATRIX_H
#define GAME_CMATRIX_H

#include "CVector3.h"

// Descriptive storage names, not recovered historical member/type names.
struct CMatrixRow {
    float x, y, z, w;
};

class CMatrix {
public:
    CMatrixRow row[4];

    static int s_ClassInit;
    static CMatrix s_TempMat; // Original 64-byte scratch matrix; no data credit.
    static void InitClass();

    CMatrix& operator=(const CMatrix& other);
    void GetSlot(unsigned int slot);
    void BuildRot(const CVector3& axis, float angle);
    void Rotate(const CVector3& axis, float angle); // Original external body.
    void Perspective(float x, float y, float near, float far);
    void Orthographic(float x, float y, float near, float far);
    void Inverse(const CMatrix& in);
    void FastInverse(const CMatrix& in);
    void ToEulerXYZ(float&, float&, float&) const;
    void Orthonormalize();
    void Multiply(const CMatrix& a, const CMatrix& b);
    void BuildRotX(float angle);
    void BuildRotY(float angle);
    void BuildRotZ(float angle);
    void BuildScale(float scale);
    void BuildScale(const CVector3& scale);
    void BuildTrans(const CVector3& position);
    void SetRight(const CVector3& right);
    void SetFront(const CVector3& front);
    void SetUp(const CVector3& up);
    void SetPos(const CVector3& position);
    // Inline row reads; names mirror the original setters and are not recovered.
    CVector3 GetRight() const { return *reinterpret_cast<const CVector3*>(&row[0]); }
    CVector3 GetFront() const { return *reinterpret_cast<const CVector3*>(&row[1]); }
    CVector3 GetUp() const { return *reinterpret_cast<const CVector3*>(&row[2]); }
    CVector3 GetPos() const { return *reinterpret_cast<const CVector3*>(&row[3]); }
    void PreTranslate(const CVector3& offset);
    void Translate(const CVector3& offset);
};

typedef char CMatrixStorageSizeCheck[sizeof(CMatrix) == 64 ? 1 : -1];

// A float view of a vector, kept for the accepted fragments that predate CVector3.h.
inline const float* Vector3Components(const CVector3& vector) {
    return reinterpret_cast<const float*>(&vector);
}

#endif
