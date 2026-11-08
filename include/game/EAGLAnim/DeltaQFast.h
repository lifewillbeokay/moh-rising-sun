// Scoped target interface; reference names adapted from CC0 nfsmw.
// See docs/Animation.md and src/eagl_anim/NOTICE for evidence and limits.
#ifndef MOH_DELTAQFAST_H
#define MOH_DELTAQFAST_H
#include "AnimCore.h"
#include "EAGLTransform.h"
namespace EAGLAnim {
#if defined(DECOMP_COMPARE)
static inline void LerpQ(const COORD4 &a, const COORD4 &b, float scale, COORD4 &q) {
    q.x = a.x + scale * (b.x - a.x);
    q.y = a.y + scale * (b.y - a.y);
    q.z = a.z + scale * (b.z - a.z);
    q.w = a.w + scale * (b.w - a.w);
}
#endif
struct DeltaQFastMinRangef {
    COORD4 mMin;
    COORD4 mRange;
};
struct DeltaQFastMinRange {
    unsigned short mMin[4];
    unsigned short mRange[4];
    void UnQuantize(DeltaQFastMinRangef &f) const {
        const float factor = 2.0f / 65535.0f;
        f.mMin.x = factor * mMin[0] - 1.0f;
        f.mMin.y = factor * mMin[1] - 1.0f;
        f.mMin.z = factor * mMin[2] - 1.0f;
        f.mMin.w = factor * mMin[3] - 1.0f;
        f.mRange.x = factor * mRange[0];
        f.mRange.y = factor * mRange[1];
        f.mRange.z = factor * mRange[2];
        f.mRange.w = factor * mRange[3];
    }
};
struct DeltaQFastPhysical {
    unsigned short mX : 12;
    unsigned char mW0 : 4;
    unsigned short mY : 12;
    unsigned char mW1 : 4;
    unsigned short mZ : 12;
    unsigned char mW2 : 4;
    void UnQuantize(COORD4 &q) const {
        q.x = mX * (2.0f / 4095.0f) - 1.0f;
        q.y = mY * (2.0f / 4095.0f) - 1.0f;
        q.z = mZ * (2.0f / 4095.0f) - 1.0f;
        unsigned short w = (mW0 << 8) + (mW1 << 4) | mW2;
        q.w = w * (2.0f / 4095.0f) - 1.0f;
    }
};
struct DeltaQFastDelta {
    unsigned char mX : 6, mW0 : 2, mY : 6, mW1 : 2, mZ : 6, mW2 : 2;
    void UnQuantize(const DeltaQFastMinRangef &f, COORD4 &q) const {
        q.x = f.mRange.x * (1.0f / 63.0f) * mX + f.mMin.x;
        q.y = f.mRange.y * (1.0f / 63.0f) * mY + f.mMin.y;
        q.z = f.mRange.z * (1.0f / 63.0f) * mZ + f.mMin.z;
        q.w = f.mRange.w * (1.0f / 63.0f) * ((mW0 << 4) + (mW1 << 2) | mW2) + f.mMin.w;
    }
};
struct DeltaQFast : AnimMemoryMap {
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
    unsigned int GetBinLength() const { return 1 << mBinLengthPower; }
    unsigned char GetBinLengthPower() const { return mBinLengthPower; }
    unsigned int GetBinLengthModMask() const { return 0x7fffffffu >> (31 - mBinLengthPower); }
    int GetBinSize() const { return AlignSize2(mNumBones * (6 + 3 * (GetBinLength() - 1))); }
    DeltaQFastMinRange *GetMinRange() {
        return reinterpret_cast<DeltaQFastMinRange *>(reinterpret_cast<unsigned char *>(this) + 18);
    }
    DeltaQFastPhysical *GetPhysical(unsigned char *binData) {
        return reinterpret_cast<DeltaQFastPhysical *>(binData);
    }
    unsigned char *GetConstBoneIdx();
    DeltaQFastPhysical *GetConstPhysical();
};
} // namespace EAGLAnim
#endif
