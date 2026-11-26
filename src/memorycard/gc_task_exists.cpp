// AI-assisted reconstruction; see docs/research/card-tasks.md.
#include "RealmcTaskUpdate.h"
namespace Realmc {
#ifdef DECOMP_COMPARE
void GCInterface::UpdateTaskCardExists() {
    int status = mpDriver->CardExists(mTaskCardExists.card) ? 2 : 0;
    if (mTaskCardExists.enable) {
        mTaskMsg.card = mTaskCardExists.card;
        mTaskMsg.Set(8, status);
    }
    mTaskCardExists.manager->EndTask(&mTaskCardExists);
}
#endif
}
