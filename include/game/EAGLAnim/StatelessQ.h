// Scoped stateless animation control interface; evaluator bodies remain external.
// Layouts/signatures follow pinned target evidence; member names follow the CC0 nfsmw reference.
// See docs/Animation.md and src/eagl_anim/NOTICE.
#ifndef MOH_STATELESSQ_H
#define MOH_STATELESSQ_H
#include "AnimCore.h"
#include "EAGLTransform.h"
namespace EAGLAnim {
class FnStatelessF3;
struct StatelessQ : public AnimMemoryMap {
    AttributeBlock *mAttributeBlock;
    unsigned short *mTimes;
    unsigned char *mBoneIdxs;
    FnStatelessF3 *mF3Ptr;
    unsigned short mNumKeys;
    unsigned char mNumBones, mNumConstBones;
#if defined(DECOMP_COMPARE)
    // Target-derived format helpers; see the partial evaluator evidence.
    unsigned short *GetData() { return reinterpret_cast<unsigned short *>(this + 1); }
    unsigned short *GetFrameData(unsigned short *data, int key) { return data + key * 4 * mNumBones; }
    unsigned short *GetConstData(unsigned short *data) { return data + mNumKeys * 4 * mNumBones; }
    unsigned char *GetConstBoneIdx() { return mBoneIdxs + mNumBones; }
    static float Uncompress2Float(unsigned short val) {
        union { unsigned int bits; float value; } result;
        result.bits = ((val & 0x8000) << 16) | ((val & 0x7fff) << 15);
        return result.value;
    }
#endif
    int GetNumFrames() const {
        if (!mTimes)
            return mNumKeys;
        return mTimes[mNumKeys - 2] + 1;
    }
};
} // namespace EAGLAnim
#endif
