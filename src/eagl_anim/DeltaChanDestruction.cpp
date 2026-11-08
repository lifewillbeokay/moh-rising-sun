// AI-assisted reconstruction from GR8E69; see docs/Animation.md.
// Base cleanup follows the attributed CC0 DeltaChan.cpp reconstruction.
#include "EAGLAnim/DeltaChan.h"
namespace EAGLAnim {
// Scoped inline definitions let this fragment reproduce the derived destructors'
// inherited cleanup. The out-of-line base bodies remain in DeltaChan.cpp; keep
// their behavior in sync. These definitions emit no additional base functions.
inline FnDeltaChan::~FnDeltaChan() {
    if (mPrevValues) {
        MemoryPoolManager::DeleteBlock(mPrevValues);
    }
    if (mDofMask) {
        MemoryPoolManager::DeleteBlock(mDofMask);
    }
}
inline FnKeyDeltaChan::~FnKeyDeltaChan() {
    if (mPrevValues) {
        MemoryPoolManager::DeleteBlock(mPrevValues);
    }
    if (mDofMask) {
        MemoryPoolManager::DeleteBlock(mDofMask);
    }
}

FnDeltaLerpChan::~FnDeltaLerpChan() {}
FnDeltaQuatChan::~FnDeltaQuatChan() {}
FnKeyLerpChan::~FnKeyLerpChan() {}
FnKeyQuatChan::~FnKeyQuatChan() {}
}
