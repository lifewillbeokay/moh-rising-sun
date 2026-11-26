// AI-assisted reconstruction; see docs/research/card-tasks.md.
#include "RealmcTaskUpdate.h"
namespace Realmc {
#ifdef DECOMP_COMPARE
void GCInterface::UpdateTaskGetCardInfo() {
    int bytes = 0, files = 0;
    int status = mpDriver->GetFreeCardSpace(mTaskGetCardInfo.card, &bytes, &files);
    if (mTaskGetCardInfo.enable) {
        mTaskMsg.Set(9, status != 0);
        mTaskMsg.freeBytes = bytes;
        mTaskMsg.freeFiles = files;
    }
    mTaskGetCardInfo.manager->EndTask(&mTaskGetCardInfo);
}
#endif
}
