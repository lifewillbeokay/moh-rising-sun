// Scoped target interface; reference names adapted from CC0 nfsmw.
// See docs/Animation.md and src/eagl_anim/NOTICE for evidence and limits.
#ifndef MOH_FNDELTAQ_H
#define MOH_FNDELTAQ_H
#pragma interface
#include "DeltaQ.h"
namespace EAGLAnim {
class FnDeltaQ : public FnAnimMemoryMap {
  public:
#include "AnimAllocation.h"
    FnDeltaQ()
        : mMinRanges(0), mBins(0), mBinSize(-1), mPrevKey(-1), mPrevQBlock(0), mPrevQs(0),
          mConstBoneIdxs(0), mConstPhysical(0) {
        mType = static_cast<AnimTypeId::Type>(17);
    }
    virtual ~FnDeltaQ() {
        if (mPrevQBlock)
            MemoryPoolManager::DeleteBlock(mPrevQBlock);
    }
    virtual void SetAnimMemoryMap(AnimMemoryMap *anim) { mpAnim = anim; }
    virtual bool GetLength(float &length) const {
        DeltaQ *deltaQ = reinterpret_cast<DeltaQ *>(mpAnim);
        length = static_cast<float>(deltaQ->GetNumFrames());
        return true;
    }
    virtual void Eval(float prevTime, float currTime, float *evalBuffer) {
        EvalSQTMasked(currTime, 0, evalBuffer);
    }
    virtual bool EvalSQT(float, float *, const BoneMask *);
    virtual bool GetAnimatedBonesAux(BoneMask *boneMask, int &numBones, int &minBone,
                                     int &maxBone) {
        DeltaQ *deltaQ = reinterpret_cast<DeltaQ *>(mpAnim);
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

  protected:
    virtual bool EvalSQTMasked(float, const BoneMask *, float *);
#if defined(DECOMP_COMPARE)
    void InitBuffersAsRequired() {
        if (!mBins) {
            DeltaQ *deltaQ = reinterpret_cast<DeltaQ *>(mpAnim);
            DeltaQMinRange *ranges = deltaQ->GetMinRange();
            mBins = reinterpret_cast<unsigned char *>(ranges + deltaQ->mNumBones);
            mConstBoneIdxs = deltaQ->GetConstBoneIdx();
            mConstPhysical = deltaQ->GetConstPhysical();
            mBinSize = deltaQ->GetBinSize();
            if (deltaQ->mNumBones) {
                void *block = MemoryPoolManager::NewBlock(deltaQ->mNumBones * sizeof(COORD4));
                mMinRanges = ranges;
                mPrevQBlock = block;
                mPrevQs = reinterpret_cast<COORD4 *>(block);
            }
        }
    }
#endif
    DeltaQMinRange *mMinRanges;
    unsigned char *mBins;
    int mBinSize, mPrevKey;
    void *mPrevQBlock;
    COORD4 *mPrevQs;
    unsigned char *mConstBoneIdxs;
    DeltaQPhysical *mConstPhysical;
};
} // namespace EAGLAnim
#endif
