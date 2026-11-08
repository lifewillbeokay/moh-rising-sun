// Scoped target interface; reference names adapted from CC0 nfsmw.
// See docs/Animation.md and src/eagl_anim/NOTICE for evidence and limits.
#ifndef MOH_DELTAQ_H
#define MOH_DELTAQ_H
#include "AnimCore.h"
#include "EAGLTransform.h"
#if defined(DECOMP_COMPARE)
extern "C" float sqrtf(float);
#endif
namespace EAGLAnim {
#if defined(DECOMP_COMPARE)
inline void DeltaQRecoverW(int signBit, COORD4 &q) {
    float ndotn = q.x * q.x + q.y * q.y + q.z * q.z;
    if (ndotn > 1.0f) {
        float len = sqrtf(ndotn);
        q.x /= len;
        q.y /= len;
        q.z /= len;
        q.w = 0.0f;
    } else {
        q.w = sqrtf(1.0f - ndotn);
        if (signBit > 0)
            q.w = -q.w;
    }
}
static inline void LerpNormalizedQ(const COORD4 &a, const COORD4 &b, float scale, COORD4 &result) {
    COORD4 q;
    q.x = a.x + scale * (b.x - a.x);
    q.y = a.y + scale * (b.y - a.y);
    q.z = a.z + scale * (b.z - a.z);
    q.w = a.w + scale * (b.w - a.w);
    float factor = 1.0f / sqrtf(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
    q.x *= factor;
    q.y *= factor;
    q.z *= factor;
    q.w *= factor;
    result = q;
}
struct DeltaQMinRangef {
    COORD3 mMin;
    COORD3 mRange;
};
struct DeltaQMinRange {
    unsigned short mMin[3], mRange[3];
    void UnQuantize(DeltaQMinRangef &f) const {
        const float factor = 2.0f / 65535.0f;
        f.mMin.x = mMin[0] * factor - 1.0f;
        f.mMin.y = mMin[1] * factor - 1.0f;
        f.mMin.z = mMin[2] * factor - 1.0f;
        f.mRange.x = mRange[0] * factor;
        f.mRange.y = mRange[1] * factor;
        f.mRange.z = mRange[2] * factor;
    }
};
struct DeltaQPhysical {
    unsigned short mX : 15, mW : 1;
    unsigned short mY, mZ;
    void UnQuantize(COORD4 &q) const {
        const float RangeScale15Bit = 2.0f / 32767.0f;
        const float RangeScale16Bit = 2.0f / 65535.0f;
        q.x = mX * RangeScale15Bit - 1.0f;
        q.y = mY * RangeScale16Bit - 1.0f;
        q.z = mZ * RangeScale16Bit - 1.0f;
        DeltaQRecoverW(mW, q);
    }
};
struct DeltaQDelta {
    unsigned char mX : 7, mW : 1;
    unsigned char mY, mZ;
    void UnQuantize(const DeltaQMinRangef &f, COORD4 &q) const {
        q.x = f.mRange.x * (1.0f / 127.0f) * mX + f.mMin.x;
        q.y = f.mRange.y * (1.0f / 255.0f) * mY + f.mMin.y;
        q.z = f.mRange.z * (1.0f / 255.0f) * mZ + f.mMin.z;
    }
};
#endif
struct DeltaQMinRange;
struct DeltaQPhysical;
struct DeltaQ : AnimMemoryMap {
    unsigned short mNumKeys;
    unsigned char mNumBones, mNumConstBones;
    unsigned short *mTimes;
    unsigned char *mBoneIdxs;
    unsigned char mBinLengthPower, mPadding;
    int GetNumFrames() const {
        if (!mTimes)
            return mNumKeys;
        else
            return mTimes[mNumKeys - 2] + 1;
    }
#if defined(DECOMP_COMPARE)
    unsigned int GetBinLength() const { return 1 << mBinLengthPower; }
    unsigned char GetBinLengthPower() const { return mBinLengthPower; }
    unsigned int GetBinLengthModMask() const {
        unsigned int result = 0x7fffffffu >> (31 - mBinLengthPower);
        return result;
    }
    int GetBinSize() const { return AlignSize2(mNumBones * (6 + 3 * (GetBinLength() - 1))); }
    DeltaQMinRange *GetMinRange() {
        return reinterpret_cast<DeltaQMinRange *>(reinterpret_cast<unsigned char *>(this) + 18);
    }
    DeltaQPhysical *GetPhysical(unsigned char *binData) {
        return reinterpret_cast<DeltaQPhysical *>(binData);
    }
    unsigned char *GetConstBoneIdx();
    DeltaQPhysical *GetConstPhysical();
#endif
};
} // namespace EAGLAnim
#endif
