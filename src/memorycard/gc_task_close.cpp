// AI-assisted reconstruction; see docs/research/card-tasks.md.
#include "RealmcTaskUpdate.h"
namespace Realmc {
void GCInterface::UpdateTaskClose() {
    int status = mpDriver->CloseFile((OpenFileDescriptor *)mTaskClose.args[0]);
    mTaskClose.manager->EndTask(&mTaskClose);
    if (mTaskClose.enable) mTaskMsg.Set(14, status != 0);
}
}
