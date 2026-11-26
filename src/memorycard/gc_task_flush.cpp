// AI-assisted reconstruction; see docs/research/card-tasks.md.
#include "RealmcTaskUpdate.h"
namespace Realmc {
#ifdef DECOMP_COMPARE
void GCInterface::UpdateTaskFlush() {
    int status = mpDriver->FlushWriteBuffer((OpenFileDescriptor *)mTaskFlush.args[0]);
    mTaskFlush.manager->EndTask(&mTaskFlush);
    if (mTaskFlush.enable) mTaskMsg.Set(20, status != 0);
}
#endif
}
