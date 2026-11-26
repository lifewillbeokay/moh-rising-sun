// AI-assisted reconstruction; see docs/research/card-tasks.md.
#include "RealmcTaskUpdate.h"
namespace Realmc {
void GCInterface::UpdateTaskSeek() {
    int status = 1;
    switch (mpDriver->Seek((OpenFileDescriptor *)mTaskSeek.args[0],
            mTaskSeek.args[1], (SeekFrom)mTaskSeek.args[2])) {
    case 0: status = 0; break;
    case 11: status = 23; break;
    }
    if (mTaskSeek.enable) mTaskMsg.Set(17, status);
    mTaskSeek.manager->EndTask(&mTaskSeek);
}
}
