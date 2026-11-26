// AI-assisted reconstruction; see docs/research/card-tasks.md.
#include "RealmcTaskUpdate.h"
namespace Realmc {
#ifdef DECOMP_COMPARE
void GCInterface::UpdateTaskRead() {
    int transferred = 0;
    int status;
    switch (mpDriver->ReadFile((OpenFileDescriptor *)mTaskRead.args[0],
            (void *)mTaskRead.args[1], mTaskRead.args[2], &transferred)) {
    case 0: status = 0; break;
    case 2: status = 6; break;
    case 3: status = 2; break;
    case 4: status = 10; break;
    case 5: status = 5; break;
    case 6: status = 14; break;
    case 8: status = 7; break;
    case 9: status = 9; break;
    case 10: status = 8; break;
    case 11: status = 23; break;
    default: status = 1; break;
    }
    if (status) mReadFailed = 1;
    if (mTaskRead.enable) {
        mTaskMsg.readBytes = transferred;
        mTaskMsg.Set(18, status);
    }
    mTaskRead.manager->EndTask(&mTaskRead);
}
#endif
}
