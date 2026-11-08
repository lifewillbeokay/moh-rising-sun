// Scoped target interface; reference names adapted from CC0 nfsmw.
// See docs/Animation.md and src/eagl_anim/NOTICE for evidence and limits.
#ifndef MOH_FNDELTAQFAST_H
#define MOH_FNDELTAQFAST_H
#pragma interface
#include "DeltaQFast.h"
namespace EAGLAnim {
class FnDeltaQFast : public FnAnimMemoryMap {
  public:
#include "AnimAllocation.h"
    FnDeltaQFast()
        : mNextKey(-1), mBoneMask(0), mMinRangesf(0), mBins(0), mBinSize(-1), mPrevKey(-1),
          mPrevQBlock(0), mPrevQs(0), mNextQBlock(0), mNextQs(0), mConstBoneIdxs(0),
          mConstPhysical(0) {
        mType = static_cast<AnimTypeId::Type>(18);
    }
    virtual ~FnDeltaQFast() {
        if (mMinRangesf)
            MemoryPoolManager::DeleteBlock(mMinRangesf);
    }
    virtual void SetAnimMemoryMap(AnimMemoryMap *anim);
    void InitBuffers(DeltaQFast *deltaQ) {
        int n = deltaQ->mNumBones;
        if (n) {
            int qBytes = n * sizeof(COORD4);
            int rangeBytes = n * sizeof(DeltaQFastMinRangef);
            unsigned char *mem = reinterpret_cast<unsigned char *>(
                MemoryPoolManager::NewBlock(qBytes + qBytes + rangeBytes));
            mMinRangesf = reinterpret_cast<DeltaQFastMinRangef *>(mem);
            mPrevQBlock = mem + rangeBytes;
            mPrevQs = reinterpret_cast<COORD4 *>(mPrevQBlock);
            mNextQBlock = mem + rangeBytes + qBytes;
            mNextQs = reinterpret_cast<COORD4 *>(mNextQBlock);
            DeltaQFastMinRange *ranges = deltaQ->GetMinRange();
            for (int i = 0; i < deltaQ->mNumBones; i++) {
                ranges[i].UnQuantize(mMinRangesf[i]);
            }
        }
    }
    virtual bool GetLength(float &length) const {
        DeltaQFast *deltaQ = reinterpret_cast<DeltaQFast *>(mpAnim);
        length = static_cast<float>(deltaQ->GetNumFrames());
        return true;
    }
    virtual void Eval(float prevTime, float currTime, float *evalBuffer) {
        EvalSQT(currTime, evalBuffer, 0);
    }
    virtual bool EvalSQT(float, float *, const BoneMask *);
    virtual bool GetAnimatedBonesAux(BoneMask *boneMask, int &numBones, int &minBone,
                                     int &maxBone) {
        DeltaQFast *deltaQ = reinterpret_cast<DeltaQFast *>(mpAnim);
        for (int i = deltaQ->mNumBones - 1; i >= 0; i--) {
            unsigned char boneIdx = deltaQ->mBoneIdxs[i];
            if (!boneMask->GetBone(boneIdx)) {
                if (numBones == 0) {
                    minBone = boneIdx;
                    maxBone = boneIdx;
                } else {
                    if (boneIdx < minBone)
                        minBone = boneIdx;
                    if (boneIdx > maxBone)
                        maxBone = boneIdx;
                }
                numBones++;
                boneMask->SetBone(boneIdx, true);
            }
        }
        return true;
    }

#if defined(DECOMP_COMPARE)
    void AddDelta(DeltaQFastPhysical *floorPhys, DeltaQFast *deltaQ, int prevDeltaIdx,
                  int floorDeltaIdx, COORD4 *prevQs) {
        DeltaQFastDelta *d = reinterpret_cast<DeltaQFastDelta *>(floorPhys + deltaQ->mNumBones) +
                             prevDeltaIdx * deltaQ->mNumBones;
        COORD4 delta;
        for (int iframe = prevDeltaIdx; iframe < floorDeltaIdx; iframe++) {
            for (int ibone = 0; ibone < deltaQ->mNumBones; ibone++) {
                d->UnQuantize(mMinRangesf[ibone], delta);
                prevQs[ibone].x += delta.x;
                prevQs[ibone].y += delta.y;
                prevQs[ibone].z += delta.z;
                prevQs[ibone].w += delta.w;
                d++;
            }
        }
    }
    void SubDelta(DeltaQFastPhysical *floorPhys, DeltaQFast *deltaQ, int prevDeltaIdx,
                  int floorDeltaIdx, COORD4 *prevQs) {
        DeltaQFastDelta *d = reinterpret_cast<DeltaQFastDelta *>(floorPhys + deltaQ->mNumBones) +
                             (prevDeltaIdx * deltaQ->mNumBones - 1);
        COORD4 delta;
        for (int iframe = prevDeltaIdx - 1; iframe >= floorDeltaIdx; iframe--) {
            for (int ibone = deltaQ->mNumBones - 1; ibone >= 0; ibone--) {
                d->UnQuantize(mMinRangesf[ibone], delta);
                prevQs[ibone].x -= delta.x;
                prevQs[ibone].y -= delta.y;
                prevQs[ibone].z -= delta.z;
                prevQs[ibone].w -= delta.w;
                d--;
            }
        }
    }
#endif
    void UpdateNextQs(DeltaQFast *, int, int, int);
    bool EvalSQTMask(float, float *, const BoneMask *);
    void AddDeltaMask(DeltaQFastPhysical *, DeltaQFast *, int, int, COORD4 *, const BoneMask *);
    void SubDeltaMask(DeltaQFastPhysical *, DeltaQFast *, int, int, COORD4 *, const BoneMask *);
    void UpdateNextQsMask(DeltaQFast *, int, int, int, const BoneMask *);
    DeltaQFastMinRangef *mMinRangesf;
    unsigned char *mBins;
    int mBinSize;
    int mPrevKey;
    void *mPrevQBlock;
    COORD4 *mPrevQs;
    int mNextKey;
    void *mNextQBlock;
    COORD4 *mNextQs;
    unsigned char *mConstBoneIdxs;
    DeltaQFastPhysical *mConstPhysical;
    const BoneMask *mBoneMask;
};
} // namespace EAGLAnim
#endif
