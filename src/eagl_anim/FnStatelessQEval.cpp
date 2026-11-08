// AI-assisted partial reconstruction from GR8E69; see docs/Animation.md and src/eagl_anim/NOTICE.
#if defined(DECOMP_COMPARE)
#include "EAGLAnim/FnStatelessQ.h"
namespace EAGLAnim {
union QuaternionBits {
    COORD4 q;
    unsigned int bits[4];
};
static inline unsigned int ExpandComponent(unsigned short val) {
    return ((val & 0x8000) << 16) | ((val & 0x7fff) << 15);
}
bool FnStatelessQ::EvalSQT(float currTime, float *sqt, const BoneMask *boneMask) {
    if (mUseFPS)
        currTime *= mFPS;
    int floorTime = FloatToInt(currTime);
    StatelessQ *anim = reinterpret_cast<StatelessQ *>(mpAnim);
    int floorKey;
    bool lerpReqd;
    float scale = 0.0f;
    if (!anim->mTimes) {
        if (floorTime < 0)
            floorKey = 0;
        else if (floorTime >= anim->mNumKeys)
            floorKey = anim->mNumKeys - 1;
        else
            floorKey = floorTime;
        lerpReqd = currTime != floorTime;
        scale = currTime - floorTime;
    } else {
        if (floorTime < anim->mTimes[0])
            floorKey = 0;
        else {
            int timeIndex = 0;
            if (mPrevKey != 0)
                timeIndex = mPrevKey - 1;
            if (anim->mTimes[timeIndex] <= floorTime) {
                while (timeIndex < anim->mNumKeys - 2 && anim->mTimes[timeIndex + 1] <= floorTime)
                    timeIndex++;
            } else {
                while (timeIndex > 0 && anim->mTimes[timeIndex] > floorTime)
                    timeIndex--;
            }
            floorKey = timeIndex + 1;
        }
        if (floorKey == 0) {
            lerpReqd = currTime != 0.0f;
            if (lerpReqd)
                scale = currTime / static_cast<float>(anim->mTimes[0]);
        } else {
            float floorKeyTime = static_cast<float>(anim->mTimes[floorKey - 1]);
            lerpReqd = currTime != floorKeyTime;
            if (lerpReqd)
                scale = (currTime - floorKeyTime) /
                        (static_cast<float>(anim->mTimes[floorKey]) - floorKeyTime);
        }
    }
    if (floorKey >= anim->mNumKeys - 1)
        lerpReqd = false;
    mPrevKey = floorKey;
    if (!boneMask) {
        if (mBoneMask)
            mBoneMask = 0;
        float *out = sqt + 4;
        unsigned short *data = anim->GetData();
        int numBones = anim->mNumBones;
        unsigned short *floorData = anim->GetFrameData(data, floorKey);
        unsigned char *boneIdxs = anim->mBoneIdxs;
        int i, dofIdx;
        if (lerpReqd && floorKey < anim->mNumKeys - 1) {
            unsigned short *ceilData = anim->GetFrameData(data, floorKey + 1);
            for (i = 0; i < numBones; i++) {
                QuaternionBits a, b;
                a.bits[0] = ExpandComponent(*floorData++);
                a.bits[1] = ExpandComponent(*floorData++);
                a.bits[2] = ExpandComponent(*floorData++);
                a.bits[3] = ExpandComponent(*floorData++);
                b.bits[0] = ExpandComponent(*ceilData++);
                b.bits[1] = ExpandComponent(*ceilData++);
                b.bits[2] = ExpandComponent(*ceilData++);
                b.bits[3] = ExpandComponent(*ceilData++);
                dofIdx = boneIdxs[i] * 12;
                float x = scale * (b.q.x - a.q.x) + a.q.x;
                float y = scale * (b.q.y - a.q.y) + a.q.y;
                float z = scale * (b.q.z - a.q.z) + a.q.z;
                float w = scale * (b.q.w - a.q.w) + a.q.w;
                COORD4 &q = *reinterpret_cast<COORD4 *>(&out[dofIdx]);
                q.x = x;
                q.y = y;
                q.z = z;
                q.w = w;
            }
        } else {
            for (i = 0; i < numBones; i++) {
                dofIdx = boneIdxs[i] * 12;
                out[dofIdx++] = anim->Uncompress2Float(*floorData++);
                out[dofIdx++] = anim->Uncompress2Float(*floorData++);
                out[dofIdx++] = anim->Uncompress2Float(*floorData++);
                out[dofIdx++] = anim->Uncompress2Float(*floorData++);
            }
        }
        numBones = anim->mNumConstBones;
        if (numBones > 0) {
            unsigned short *constData = anim->GetConstData(data);
            unsigned char *constIdxs = anim->GetConstBoneIdx();
            for (i = 0; i < numBones; i++) {
                dofIdx = constIdxs[i] * 12;
                out[dofIdx++] = anim->Uncompress2Float(*constData++);
                out[dofIdx++] = anim->Uncompress2Float(*constData++);
                out[dofIdx++] = anim->Uncompress2Float(*constData++);
                out[dofIdx++] = anim->Uncompress2Float(*constData++);
            }
        }
    } else {
        EvalSQTMask(currTime, sqt, boneMask, lerpReqd, floorKey, scale);
    }
    if (anim->mF3Ptr)
        anim->mF3Ptr->EvalSQTfast(currTime, sqt, boneMask, lerpReqd, floorKey, scale);
    return true;
}
bool FnStatelessQ::EvalSQTMask(float currTime, float *sqt, const BoneMask *boneMask, bool lerpReqd,
                               int floorKey, float scale) {
    if (boneMask != mBoneMask)
        mBoneMask = boneMask;
    StatelessQ *anim = reinterpret_cast<StatelessQ *>(mpAnim);
    float *out = sqt + 4;
    unsigned short *data = anim->GetData();
    int numBones = anim->mNumBones;
    unsigned short *floorData = anim->GetFrameData(data, floorKey);
    unsigned char *boneIdxs = anim->mBoneIdxs;
    int i, dofIdx;
    if (lerpReqd && floorKey < anim->mNumKeys - 1) {
        unsigned short *ceilData = anim->GetFrameData(data, floorKey + 1);
        for (i = 0; i < numBones; i++) {
            if (boneMask->GetBone(boneIdxs[i])) {
                unsigned short *floorPtr = floorData + i * 4;
                unsigned short *ceilPtr = ceilData + i * 4;
                QuaternionBits a, b;
                a.bits[0] = ExpandComponent(*floorPtr++);
                a.bits[1] = ExpandComponent(*floorPtr++);
                a.bits[2] = ExpandComponent(*floorPtr++);
                a.bits[3] = ExpandComponent(*floorPtr++);
                b.bits[0] = ExpandComponent(*ceilPtr++);
                b.bits[1] = ExpandComponent(*ceilPtr++);
                b.bits[2] = ExpandComponent(*ceilPtr++);
                b.bits[3] = ExpandComponent(*ceilPtr++);
                dofIdx = boneIdxs[i] * 12;
                float x = scale * (b.q.x - a.q.x) + a.q.x;
                float y = scale * (b.q.y - a.q.y) + a.q.y;
                float z = scale * (b.q.z - a.q.z) + a.q.z;
                float w = scale * (b.q.w - a.q.w) + a.q.w;
                COORD4 &q = *reinterpret_cast<COORD4 *>(&out[dofIdx]);
                q.x = x;
                q.y = y;
                q.z = z;
                q.w = w;
            }
        }
    } else {
        for (i = 0; i < numBones; i++) {
            if (boneMask->GetBone(boneIdxs[i])) {
                unsigned short *floorPtr = floorData + i * 4;
                dofIdx = boneIdxs[i] * 12;
                out[dofIdx++] = anim->Uncompress2Float(*floorPtr++);
                out[dofIdx++] = anim->Uncompress2Float(*floorPtr++);
                out[dofIdx++] = anim->Uncompress2Float(*floorPtr++);
                out[dofIdx++] = anim->Uncompress2Float(*floorPtr++);
            }
        }
    }
    numBones = anim->mNumConstBones;
    if (numBones > 0) {
        unsigned short *constData = anim->GetConstData(data);
        unsigned char *constIdxs = anim->GetConstBoneIdx();
        for (i = 0; i < numBones; i++) {
            if (boneMask->GetBone(constIdxs[i])) {
                dofIdx = constIdxs[i] * 12;
                out[dofIdx++] = anim->Uncompress2Float(*constData++);
                out[dofIdx++] = anim->Uncompress2Float(*constData++);
                out[dofIdx++] = anim->Uncompress2Float(*constData++);
                out[dofIdx++] = anim->Uncompress2Float(*constData++);
            }
        }
    }
    return true;
}
} // namespace EAGLAnim

#endif
