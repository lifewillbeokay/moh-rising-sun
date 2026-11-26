// AI-assisted reconstruction; see docs/research/card-tasks.md.
#include "RealmcTaskUpdate.h"
namespace Realmc {
#ifdef DECOMP_COMPARE
void GCInterface::UpdateTaskOpen() {
    OpenFileDescriptor *file = 0;
    int status = 1;
    switch (mpDriver->OpenFile(mTaskOpen.card, (FileInfo *)mTaskOpen.args[0],
            (FileOpenMode)mTaskOpen.args[1], &file)) {
    case 0: status = 0; break;
    case 4: status = 11; break;
    case 5: status = 5; break;
    case 6: status = 14; break;
    }
    mTaskOpen.manager->EndTask(&mTaskOpen);
    if (mTaskOpen.enable) {
        mTaskMsg.Set(13, status);
        mTaskMsg.file = file;
        mTaskMsg.name = ((FileInfo *)mTaskOpen.args[0])->name;
    }
}
#endif
}
