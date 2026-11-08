// Scoped target reconstruction; names adapted from CC0 nfsmw.
// See docs/Animation.md and src/eagl_anim/NOTICE for evidence and limits.
// Comparison unit: unfinished evaluators plus verified context; not linked.
#if defined(DECOMP_COMPARE)
#pragma implementation "FnDeltaQ.h"
#include "EAGLAnim/FnDeltaQ.h"
namespace EAGLAnim {
bool FnDeltaQ::EvalSQT(float currTime, float *sqt, const BoneMask *boneMask) {
    if (boneMask)
        return EvalSQTMasked(currTime, boneMask, sqt);
    InitBuffersAsRequired();
    DeltaQ *deltaQ = reinterpret_cast<DeltaQ *>(mpAnim);
    if (deltaQ->mNumBones) {
        int floorTime = FloatToInt(currTime);
        int floorKey;
        if (!deltaQ->mTimes) {
            if (floorTime < 0) {
                floorKey = 0;
            } else {
                if (floorTime >= deltaQ->mNumKeys) {
                    floorKey = deltaQ->mNumKeys - 1;
                } else {
                    floorKey = floorTime;
                }
            }
        } else if (floorTime < deltaQ->mTimes[0]) {
            floorKey = 0;
        } else {
            int timeIndex;
            if (mPrevKey < 1) {
                timeIndex = 0;
            } else {
                timeIndex = mPrevKey - 1;
            }
            if (deltaQ->mTimes[timeIndex] <= floorTime) {
                while (timeIndex < deltaQ->mNumKeys - 2 &&
                       deltaQ->mTimes[timeIndex + 1] <= floorTime) {
                    timeIndex++;
                }
            } else {
                while (timeIndex > 0 && deltaQ->mTimes[timeIndex] > floorTime) {
                    timeIndex--;
                }
            }

            floorKey = timeIndex + 1;
        }

        unsigned int binLenPower = deltaQ->GetBinLengthPower();
        int floorBinIdx = floorKey >> binLenPower;
        unsigned char *binData = mBins + floorBinIdx * mBinSize;
        DeltaQPhysical *floorPhys = deltaQ->GetPhysical(binData);
        unsigned int binLenModMask = 0x7fffffffu >> (31 - binLenPower);
        int floorDeltaIdx = floorKey & binLenModMask;
        int prevBinIdx = mPrevKey >> binLenPower;
        unsigned char *boneIdxs = deltaQ->mBoneIdxs;
        int prevDeltaIdx;
        bool preventReverse = floorKey < mPrevKey && !IsReverseDeltaSumEnabled();
        if (mPrevKey == -1 || floorBinIdx != prevBinIdx || floorDeltaIdx == 0 || preventReverse) {
            for (int ibone = 0; ibone < deltaQ->mNumBones; ibone++) {
                floorPhys[ibone].UnQuantize(mPrevQs[ibone]);
            }
            prevDeltaIdx = 0;
        } else
            prevDeltaIdx = mPrevKey & binLenModMask;
        int ceilKey = floorKey + 1;
        if (prevDeltaIdx < floorDeltaIdx) {
            DeltaQDelta *d = reinterpret_cast<DeltaQDelta *>(floorPhys + deltaQ->mNumBones) +
                             prevDeltaIdx * deltaQ->mNumBones;
            COORD4 delta;
            DeltaQMinRangef ranges;
            for (int iframe = prevDeltaIdx; iframe < floorDeltaIdx; iframe++) {
                for (int ibone = 0; ibone < deltaQ->mNumBones; ibone++) {
                    mMinRanges[ibone].UnQuantize(ranges);
                    d->UnQuantize(ranges, delta);
                    mPrevQs[ibone].x += delta.x;
                    mPrevQs[ibone].y += delta.y;
                    mPrevQs[ibone].z += delta.z;
                    d++;
                }
            }
            d = reinterpret_cast<DeltaQDelta *>(floorPhys + deltaQ->mNumBones) +
                (floorDeltaIdx - 1) * deltaQ->mNumBones;
            for (int ibone = 0; ibone < deltaQ->mNumBones; ibone++) {
                DeltaQRecoverW(d->mW, mPrevQs[ibone]);
                d++;
            }
        } else if (prevDeltaIdx > floorDeltaIdx) {
            DeltaQDelta *d = reinterpret_cast<DeltaQDelta *>(floorPhys + deltaQ->mNumBones) +
                             (prevDeltaIdx * deltaQ->mNumBones - 1);
            COORD4 delta;
            DeltaQMinRangef ranges;
            for (int iframe = prevDeltaIdx - 1; iframe >= floorDeltaIdx; iframe--) {
                for (int ibone = deltaQ->mNumBones - 1; ibone >= 0; ibone--) {
                    mMinRanges[ibone].UnQuantize(ranges);
                    d->UnQuantize(ranges, delta);
                    mPrevQs[ibone].x -= delta.x;
                    mPrevQs[ibone].y -= delta.y;
                    mPrevQs[ibone].z -= delta.z;
                    d--;
                }
            }
            d = reinterpret_cast<DeltaQDelta *>(floorPhys + deltaQ->mNumBones) +
                (floorDeltaIdx - 1) * deltaQ->mNumBones;
            for (int ibone = 0; ibone < deltaQ->mNumBones; ibone++) {
                DeltaQRecoverW(d->mW, mPrevQs[ibone]);
                d++;
            }
        }
        mPrevKey = floorKey;
        float scale;
        bool lerpReqd;
        if (!deltaQ->mTimes) {
            lerpReqd = currTime != floorTime;
            if (lerpReqd) {
                scale = currTime - floorTime;
            }
        } else if (floorKey == 0) {
            lerpReqd = currTime != 0.0f;
            if (lerpReqd) {
                float ceilKeyTime = static_cast<float>(deltaQ->mTimes[floorKey]);
                scale = currTime / ceilKeyTime;
            }
        } else {
            float floorKeyTime = deltaQ->mTimes[floorKey - 1];
            lerpReqd = currTime != floorKeyTime;
            if (lerpReqd) {
                float ceilKeyTime = static_cast<float>(deltaQ->mTimes[floorKey]);
                scale = (currTime - floorKeyTime) / (ceilKeyTime - floorKeyTime);
            }
        }
        if (lerpReqd && floorKey < deltaQ->mNumKeys - 1) {
            int ceilBinIdx = ceilKey >> binLenPower;
            DeltaQPhysical *phys =
                reinterpret_cast<DeltaQPhysical *>(mBins + ceilBinIdx * mBinSize);
            COORD4 q;
            if (ceilBinIdx != floorBinIdx) {
                for (int ibone = 0; ibone < deltaQ->mNumBones; ibone++) {
                    phys[ibone].UnQuantize(q);
                    LerpNormalizedQ(mPrevQs[ibone], q, scale,
                                    *reinterpret_cast<COORD4 *>(&sqt[boneIdxs[ibone] * 12 + 4]));
                }
            } else {
                DeltaQDelta *d = reinterpret_cast<DeltaQDelta *>(phys + deltaQ->mNumBones) +
                                 floorDeltaIdx * deltaQ->mNumBones;
                DeltaQMinRangef ranges;
                for (int ibone = 0; ibone < deltaQ->mNumBones; ibone++) {
                    mMinRanges[ibone].UnQuantize(ranges);
                    d->UnQuantize(ranges, q);
                    q.x += mPrevQs[ibone].x;
                    q.y += mPrevQs[ibone].y;
                    q.z += mPrevQs[ibone].z;
                    DeltaQRecoverW(d->mW, q);
                    LerpNormalizedQ(mPrevQs[ibone], q, scale,
                                    *reinterpret_cast<COORD4 *>(&sqt[boneIdxs[ibone] * 12 + 4]));
                    d++;
                }
            }
        } else {
            for (int ibone = 0; ibone < deltaQ->mNumBones; ibone++) {
                *reinterpret_cast<COORD4 *>(&sqt[boneIdxs[ibone] * 12 + 4]) = mPrevQs[ibone];
            }
        }
    }
    if (deltaQ->mNumConstBones) {
        for (int ibone = 0; ibone < deltaQ->mNumConstBones; ibone++) {
            mConstPhysical[ibone].UnQuantize(
                *reinterpret_cast<COORD4 *>(&sqt[mConstBoneIdxs[ibone] * 12 + 4]));
        }
    }
    return true;
}
bool FnDeltaQ::EvalSQTMasked(float currTime, const BoneMask *boneMask, float *sqt) {
    InitBuffersAsRequired();
    DeltaQ *deltaQ = reinterpret_cast<DeltaQ *>(mpAnim);
    if (deltaQ->mNumBones) {
        int floorTime = FloatToInt(currTime);
        int floorKey;
        if (!deltaQ->mTimes) {
            if (floorTime < 0) {
                floorKey = 0;
            } else {
                if (floorTime >= deltaQ->mNumKeys) {
                    floorKey = deltaQ->mNumKeys - 1;
                } else {
                    floorKey = floorTime;
                }
            }
        } else if (floorTime < deltaQ->mTimes[0]) {
            floorKey = 0;
        } else {
            int timeIndex;
            if (mPrevKey < 1) {
                timeIndex = 0;
            } else {
                timeIndex = mPrevKey - 1;
            }
            if (deltaQ->mTimes[timeIndex] <= floorTime) {
                while (timeIndex < deltaQ->mNumKeys - 2 &&
                       deltaQ->mTimes[timeIndex + 1] <= floorTime) {
                    timeIndex++;
                }
            } else {
                while (timeIndex > 0 && deltaQ->mTimes[timeIndex] > floorTime) {
                    timeIndex--;
                }
            }

            floorKey = timeIndex + 1;
        }

        unsigned int binLenPower = deltaQ->GetBinLengthPower();
        int floorBinIdx = floorKey >> binLenPower;
        unsigned char *binData = mBins + floorBinIdx * mBinSize;
        DeltaQPhysical *floorPhys = deltaQ->GetPhysical(binData);
        unsigned int binLenModMask = 0x7fffffffu >> (31 - binLenPower);
        int floorDeltaIdx = floorKey & binLenModMask;
        int prevBinIdx = mPrevKey >> binLenPower;
        unsigned char *boneIdxs = deltaQ->mBoneIdxs;
        int prevDeltaIdx;
        bool preventReverse = floorKey < mPrevKey && !IsReverseDeltaSumEnabled();
        if (mPrevKey == -1 || floorBinIdx != prevBinIdx || floorDeltaIdx == 0 || preventReverse) {
            for (int ibone = 0; ibone < deltaQ->mNumBones; ibone++) {
                if (boneMask->GetBone(boneIdxs[ibone]))
                    floorPhys[ibone].UnQuantize(mPrevQs[ibone]);
            }
            prevDeltaIdx = 0;
        } else
            prevDeltaIdx = mPrevKey & binLenModMask;
        int ceilKey = floorKey + 1;
        if (prevDeltaIdx < floorDeltaIdx) {
            DeltaQDelta *d = reinterpret_cast<DeltaQDelta *>(floorPhys + deltaQ->mNumBones) +
                             prevDeltaIdx * deltaQ->mNumBones;
            COORD4 delta;
            DeltaQMinRangef ranges;
            for (int iframe = prevDeltaIdx; iframe < floorDeltaIdx; iframe++) {
                for (int ibone = 0; ibone < deltaQ->mNumBones; ibone++) {
                    if (boneMask->GetBone(boneIdxs[ibone])) {
                        mMinRanges[ibone].UnQuantize(ranges);
                        d->UnQuantize(ranges, delta);
                        mPrevQs[ibone].x += delta.x;
                        mPrevQs[ibone].y += delta.y;
                        mPrevQs[ibone].z += delta.z;
                    }
                    d++;
                }
            }
            d = reinterpret_cast<DeltaQDelta *>(floorPhys + deltaQ->mNumBones) +
                (floorDeltaIdx - 1) * deltaQ->mNumBones;
            for (int ibone = 0; ibone < deltaQ->mNumBones; ibone++) {
                if (boneMask->GetBone(boneIdxs[ibone]))
                    DeltaQRecoverW(d->mW, mPrevQs[ibone]);
                d++;
            }
        } else if (prevDeltaIdx > floorDeltaIdx) {
            DeltaQDelta *d = reinterpret_cast<DeltaQDelta *>(floorPhys + deltaQ->mNumBones) +
                             (prevDeltaIdx * deltaQ->mNumBones - 1);
            COORD4 delta;
            DeltaQMinRangef ranges;
            for (int iframe = prevDeltaIdx - 1; iframe >= floorDeltaIdx; iframe--) {
                for (int ibone = deltaQ->mNumBones - 1; ibone >= 0; ibone--) {
                    if (boneMask->GetBone(boneIdxs[ibone])) {
                        mMinRanges[ibone].UnQuantize(ranges);
                        d->UnQuantize(ranges, delta);
                        mPrevQs[ibone].x -= delta.x;
                        mPrevQs[ibone].y -= delta.y;
                        mPrevQs[ibone].z -= delta.z;
                    }
                    d--;
                }
            }
            d = reinterpret_cast<DeltaQDelta *>(floorPhys + deltaQ->mNumBones) +
                (floorDeltaIdx - 1) * deltaQ->mNumBones;
            for (int ibone = 0; ibone < deltaQ->mNumBones; ibone++) {
                if (boneMask->GetBone(boneIdxs[ibone]))
                    DeltaQRecoverW(d->mW, mPrevQs[ibone]);
                d++;
            }
        }
        mPrevKey = floorKey;
        float scale;
        bool lerpReqd;
        if (!deltaQ->mTimes) {
            lerpReqd = currTime != floorTime;
            if (lerpReqd) {
                scale = currTime - floorTime;
            }
        } else if (floorKey == 0) {
            lerpReqd = currTime != 0.0f;
            if (lerpReqd) {
                float ceilKeyTime = static_cast<float>(deltaQ->mTimes[floorKey]);
                scale = currTime / ceilKeyTime;
            }
        } else {
            float floorKeyTime = deltaQ->mTimes[floorKey - 1];
            lerpReqd = currTime != floorKeyTime;
            if (lerpReqd) {
                float ceilKeyTime = static_cast<float>(deltaQ->mTimes[floorKey]);
                scale = (currTime - floorKeyTime) / (ceilKeyTime - floorKeyTime);
            }
        }
        if (lerpReqd && floorKey < deltaQ->mNumKeys - 1) {
            int ceilBinIdx = ceilKey >> binLenPower;
            DeltaQPhysical *phys =
                reinterpret_cast<DeltaQPhysical *>(mBins + ceilBinIdx * mBinSize);
            COORD4 q;
            if (ceilBinIdx != floorBinIdx) {
                for (int ibone = 0; ibone < deltaQ->mNumBones; ibone++) {
                    if (boneMask->GetBone(boneIdxs[ibone])) {
                        phys[ibone].UnQuantize(q);
                        LerpNormalizedQ(
                            mPrevQs[ibone], q, scale,
                            *reinterpret_cast<COORD4 *>(&sqt[boneIdxs[ibone] * 12 + 4]));
                    }
                }
            } else {
                DeltaQDelta *d = reinterpret_cast<DeltaQDelta *>(phys + deltaQ->mNumBones) +
                                 floorDeltaIdx * deltaQ->mNumBones;
                DeltaQMinRangef ranges;
                for (int ibone = 0; ibone < deltaQ->mNumBones; ibone++) {
                    if (boneMask->GetBone(boneIdxs[ibone])) {
                        mMinRanges[ibone].UnQuantize(ranges);
                        d->UnQuantize(ranges, q);
                        q.x += mPrevQs[ibone].x;
                        q.y += mPrevQs[ibone].y;
                        q.z += mPrevQs[ibone].z;
                        DeltaQRecoverW(d->mW, q);
                        LerpNormalizedQ(
                            mPrevQs[ibone], q, scale,
                            *reinterpret_cast<COORD4 *>(&sqt[boneIdxs[ibone] * 12 + 4]));
                    }
                    d++;
                }
            }
        } else {
            for (int ibone = 0; ibone < deltaQ->mNumBones; ibone++) {
                if (boneMask->GetBone(boneIdxs[ibone]))
                    *reinterpret_cast<COORD4 *>(&sqt[boneIdxs[ibone] * 12 + 4]) = mPrevQs[ibone];
            }
        }
    }
    if (deltaQ->mNumConstBones) {
        unsigned char *boneIdxs = deltaQ->mBoneIdxs;
        for (int ibone = 0; ibone < deltaQ->mNumConstBones; ibone++) {
            if (boneMask->GetBone(boneIdxs[ibone]))
                mConstPhysical[ibone].UnQuantize(
                    *reinterpret_cast<COORD4 *>(&sqt[mConstBoneIdxs[ibone] * 12 + 4]));
        }
    }
    mPrevKey = -1;
    return true;
}
} // namespace EAGLAnim

#endif
