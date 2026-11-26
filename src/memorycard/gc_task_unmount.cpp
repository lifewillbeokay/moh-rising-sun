// AI-assisted reconstruction; see docs/research/card-tasks.md.
#include "RealmcTaskUpdate.h"
namespace Realmc {
#ifdef DECOMP_COMPARE
void GCInterface::UpdateTaskUnmount() {
    int status = 1;
    switch (mpDriver->Unmount(mTaskUnmount.card)) {
    case 0: status = 0; break;
    case 3: status = 2; break;
    }
    mTaskUnmount.manager->EndTask(&mTaskUnmount);
    if (mTaskUnmount.enable) mTaskMsg.Set(22, status);
}
#endif
}
