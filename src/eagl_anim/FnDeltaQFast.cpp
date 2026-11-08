// Scoped target reconstruction; names adapted from CC0 nfsmw.
// See docs/Animation.md and src/eagl_anim/NOTICE for evidence and limits.
// Comparison unit: unfinished evaluators plus verified context; not linked.
#if defined(DECOMP_COMPARE)
#pragma implementation "FnDeltaQFast.h"
#include "EAGLAnim/FnDeltaQFast.h"
namespace EAGLAnim {
void FnDeltaQFast::SetAnimMemoryMap(AnimMemoryMap *anim) {
    mpAnim = anim;
    DeltaQFast *deltaQ = reinterpret_cast<DeltaQFast *>(anim);
    DeltaQFastMinRange *ranges = deltaQ->GetMinRange();
    mBins = reinterpret_cast<unsigned char *>(ranges + deltaQ->mNumBones);
    mConstBoneIdxs = deltaQ->GetConstBoneIdx();
    mConstPhysical = deltaQ->GetConstPhysical();
    mBinSize = deltaQ->GetBinSize();
    InitBuffers(deltaQ);
}
bool FnDeltaQFast::EvalSQT(float currTime, float *sqt, const BoneMask *boneMask) {
    if (!boneMask) {
        if (mBoneMask) {
            mPrevKey = -1;
            mNextKey = -1;
            mBoneMask = 0;
        }
        DeltaQFast *deltaQ = reinterpret_cast<DeltaQFast *>(mpAnim);
        float *qOut = sqt + 4;
        unsigned char *boneIdxs = deltaQ->mBoneIdxs;
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
            unsigned int binLenModMask = deltaQ->GetBinLengthModMask();
            int floorBinIdx = floorKey >> binLenPower;
            int floorDeltaIdx = floorKey & binLenModMask;
            int prevBinIdx = mPrevKey >> binLenPower;
            int ceilKey = floorKey + 1;
            if (mNextKey == floorKey) {
                COORD4 *temp = mNextQs;
                mNextQs = mPrevQs;
                mPrevQs = temp;
                mNextKey = mPrevKey;
            } else {
                if (mPrevKey == ceilKey) {
                    for (int i = 0; i < deltaQ->mNumBones; i++)
                        mNextQs[i] = mPrevQs[i];
                    mNextKey = mPrevKey;
                }
                unsigned char *binData = mBins + floorBinIdx * mBinSize;
                DeltaQFastPhysical *floorPhys = deltaQ->GetPhysical(binData);
                int prevDeltaIdx;
                int prevKey = mPrevKey;
                bool preventReverse = floorKey < prevKey && !IsReverseDeltaSumEnabled();
                if (prevKey == -1 || floorBinIdx != prevBinIdx || floorDeltaIdx == 0 ||
                    preventReverse) {
                    for (int ibone = 0; ibone < deltaQ->mNumBones; ibone++) {
                        floorPhys[ibone].UnQuantize(mPrevQs[ibone]);
                    }
                    prevDeltaIdx = 0;
                } else
                    prevDeltaIdx = prevKey & binLenModMask;
                if (prevDeltaIdx < floorDeltaIdx)
                    AddDelta(floorPhys, deltaQ, prevDeltaIdx, floorDeltaIdx, mPrevQs);
                else if (prevDeltaIdx > floorDeltaIdx)
                    SubDelta(floorPhys, deltaQ, prevDeltaIdx, floorDeltaIdx, mPrevQs);
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
                UpdateNextQs(deltaQ, ceilKey, floorBinIdx, floorDeltaIdx);
                for (int ibone = 0; ibone < deltaQ->mNumBones; ibone++) {
                    {
                        LerpQ(mPrevQs[ibone], mNextQs[ibone], scale,
                              *reinterpret_cast<COORD4 *>(&qOut[boneIdxs[ibone] * 12]));
                    }
                }
            } else {
                for (int ibone = 0; ibone < deltaQ->mNumBones; ibone++) {
                    *reinterpret_cast<COORD4 *>(boneIdxs[ibone] * 12 + qOut) = mPrevQs[ibone];
                }
            }
        }
        if (deltaQ->mNumConstBones) {
            for (int ibone = 0; ibone < deltaQ->mNumConstBones; ibone++) {
                mConstPhysical[ibone].UnQuantize(
                    *reinterpret_cast<COORD4 *>(&qOut[mConstBoneIdxs[ibone] * 12]));
            }
        }
        return true;
    } else {
        return EvalSQTMask(currTime, sqt, boneMask);
    }
}
void FnDeltaQFast::UpdateNextQs(DeltaQFast *deltaQ, int ceilKey, int floorBinIdx,
                                int floorDeltaIdx) {
    if (ceilKey != mNextKey) {
        int ceilBinIdx = ceilKey >> deltaQ->GetBinLengthPower();
        DeltaQFastPhysical *phys =
            reinterpret_cast<DeltaQFastPhysical *>(mBins + ceilBinIdx * mBinSize);
        if (ceilBinIdx != floorBinIdx) {
            for (int ibone = 0; ibone < deltaQ->mNumBones; ibone++)
                phys[ibone].UnQuantize(mNextQs[ibone]);
        } else {
            DeltaQFastDelta *d = reinterpret_cast<DeltaQFastDelta *>(phys + deltaQ->mNumBones) +
                                 floorDeltaIdx * deltaQ->mNumBones;
            COORD4 delta;
            for (int ibone = 0; ibone < deltaQ->mNumBones; ibone++) {
                d->UnQuantize(mMinRangesf[ibone], delta);
                mNextQs[ibone].x = delta.x + mPrevQs[ibone].x;
                mNextQs[ibone].y = delta.y + mPrevQs[ibone].y;
                mNextQs[ibone].z = delta.z + mPrevQs[ibone].z;
                mNextQs[ibone].w = delta.w + mPrevQs[ibone].w;
                d++;
            }
        }
        mNextKey = ceilKey;
    }
}
bool FnDeltaQFast::EvalSQTMask(float currTime, float *sqt, const BoneMask *boneMask) {
    if (boneMask != mBoneMask) {
        mPrevKey = -1;
        mNextKey = -1;
        mBoneMask = boneMask;
    }
    DeltaQFast *deltaQ = reinterpret_cast<DeltaQFast *>(mpAnim);
    float *qOut = sqt + 4;
    unsigned char *boneIdxs = deltaQ->mBoneIdxs;
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
        unsigned int binLenModMask = deltaQ->GetBinLengthModMask();
        int floorBinIdx = floorKey >> binLenPower;
        int floorDeltaIdx = floorKey & binLenModMask;
        int prevBinIdx = mPrevKey >> binLenPower;
        int ceilKey = floorKey + 1;
        if (mNextKey == floorKey) {
            COORD4 *temp = mNextQs;
            mNextQs = mPrevQs;
            mPrevQs = temp;
            mNextKey = mPrevKey;
        } else {
            if (mPrevKey == ceilKey) {
                for (int i = 0; i < deltaQ->mNumBones; i++)
                    mNextQs[i] = mPrevQs[i];
                mNextKey = mPrevKey;
            }
            unsigned char *binData = mBins + floorBinIdx * mBinSize;
            DeltaQFastPhysical *floorPhys = deltaQ->GetPhysical(binData);
            int prevDeltaIdx;
            int prevKey = mPrevKey;
            bool preventReverse = floorKey < prevKey && !IsReverseDeltaSumEnabled();
            if (prevKey == -1 || floorBinIdx != prevBinIdx || preventReverse) {
                for (int ibone = 0; ibone < deltaQ->mNumBones; ibone++) {
                    if (boneMask->GetBone(boneIdxs[ibone]))
                        floorPhys[ibone].UnQuantize(mPrevQs[ibone]);
                }
                prevDeltaIdx = 0;
            } else
                prevDeltaIdx = prevKey & binLenModMask;
            if (prevDeltaIdx < floorDeltaIdx)
                AddDeltaMask(floorPhys, deltaQ, prevDeltaIdx, floorDeltaIdx, mPrevQs, boneMask);
            else if (prevDeltaIdx > floorDeltaIdx)
                SubDeltaMask(floorPhys, deltaQ, prevDeltaIdx, floorDeltaIdx, mPrevQs, boneMask);
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
            UpdateNextQsMask(deltaQ, ceilKey, floorBinIdx, floorDeltaIdx, boneMask);
            for (int ibone = 0; ibone < deltaQ->mNumBones; ibone++) {
                if (boneMask->GetBone(boneIdxs[ibone])) {
                    LerpQ(mPrevQs[ibone], mNextQs[ibone], scale,
                          *reinterpret_cast<COORD4 *>(&qOut[boneIdxs[ibone] * 12]));
                }
            }
        } else {
            for (int ibone = 0; ibone < deltaQ->mNumBones; ibone++) {
                if (boneMask->GetBone(boneIdxs[ibone]))
                    *reinterpret_cast<COORD4 *>(boneIdxs[ibone] * 12 + qOut) = mPrevQs[ibone];
            }
        }
    }
    if (deltaQ->mNumConstBones) {
        for (int ibone = 0; ibone < deltaQ->mNumConstBones; ibone++) {
            if (boneMask->GetBone(mConstBoneIdxs[ibone]))
                mConstPhysical[ibone].UnQuantize(
                    *reinterpret_cast<COORD4 *>(&qOut[mConstBoneIdxs[ibone] * 12]));
        }
    }
    return true;
}
void FnDeltaQFast::AddDeltaMask(DeltaQFastPhysical *floorPhys, DeltaQFast *deltaQ, int prevDeltaIdx,
                                int floorDeltaIdx, COORD4 *prevQs, const BoneMask *boneMask) {
    unsigned char *boneIdxs = deltaQ->mBoneIdxs;
    DeltaQFastDelta *d = reinterpret_cast<DeltaQFastDelta *>(floorPhys + deltaQ->mNumBones) +
                         prevDeltaIdx * deltaQ->mNumBones;
    COORD4 delta;
    for (int iframe = prevDeltaIdx; iframe < floorDeltaIdx; iframe++) {
        for (int ibone = 0; ibone < deltaQ->mNumBones; ibone++) {
            if (boneMask->GetBone(boneIdxs[ibone])) {
                d->UnQuantize(mMinRangesf[ibone], delta);
                prevQs[ibone].x += delta.x;
                prevQs[ibone].y += delta.y;
                prevQs[ibone].z += delta.z;
                prevQs[ibone].w += delta.w;
            }
            d++;
        }
    }
}
void FnDeltaQFast::SubDeltaMask(DeltaQFastPhysical *floorPhys, DeltaQFast *deltaQ, int prevDeltaIdx,
                                int floorDeltaIdx, COORD4 *prevQs, const BoneMask *boneMask) {
    unsigned char *boneIdxs = deltaQ->mBoneIdxs;
    DeltaQFastDelta *d = reinterpret_cast<DeltaQFastDelta *>(floorPhys + deltaQ->mNumBones) +
                         (prevDeltaIdx * deltaQ->mNumBones - 1);
    COORD4 delta;
    for (int iframe = prevDeltaIdx - 1; iframe >= floorDeltaIdx; iframe--) {
        for (int ibone = deltaQ->mNumBones - 1; ibone >= 0; ibone--) {
            if (boneMask->GetBone(boneIdxs[ibone])) {
                d->UnQuantize(mMinRangesf[ibone], delta);
                prevQs[ibone].x -= delta.x;
                prevQs[ibone].y -= delta.y;
                prevQs[ibone].z -= delta.z;
                prevQs[ibone].w -= delta.w;
            }
            d--;
        }
    }
}
void FnDeltaQFast::UpdateNextQsMask(DeltaQFast *deltaQ, int ceilKey, int floorBinIdx,
                                    int floorDeltaIdx, const BoneMask *boneMask) {
    if (ceilKey != mNextKey) {
        int ceilBinIdx = ceilKey >> deltaQ->GetBinLengthPower();
        DeltaQFastPhysical *phys =
            reinterpret_cast<DeltaQFastPhysical *>(mBins + ceilBinIdx * mBinSize);
        unsigned char *boneIdxs = deltaQ->mBoneIdxs;
        if (ceilBinIdx != floorBinIdx) {
            for (int ibone = 0; ibone < deltaQ->mNumBones; ibone++)
                if (boneMask->GetBone(boneIdxs[ibone]))
                    phys[ibone].UnQuantize(mNextQs[ibone]);
        } else {
            DeltaQFastDelta *d = reinterpret_cast<DeltaQFastDelta *>(phys + deltaQ->mNumBones) +
                                 floorDeltaIdx * deltaQ->mNumBones;
            COORD4 delta;
            for (int ibone = 0; ibone < deltaQ->mNumBones; ibone++) {
                if (boneMask->GetBone(boneIdxs[ibone])) {
                    d->UnQuantize(mMinRangesf[ibone], delta);
                    mNextQs[ibone].x = delta.x + mPrevQs[ibone].x;
                    mNextQs[ibone].y = delta.y + mPrevQs[ibone].y;
                    mNextQs[ibone].z = delta.z + mPrevQs[ibone].z;
                    mNextQs[ibone].w = delta.w + mPrevQs[ibone].w;
                }
                d++;
            }
        }
        mNextKey = ceilKey;
    }
}
} // namespace EAGLAnim

#endif
