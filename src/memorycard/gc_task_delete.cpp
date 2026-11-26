// AI-assisted reconstruction; see docs/research/card-tasks.md.
#include "RealmcTaskUpdate.h"
namespace Realmc {
#ifdef DECOMP_COMPARE
void GCInterface::UpdateTaskDelete() {
    // The public FileInfo occupies 48 bytes; the driver header models its used prefix.
    struct FullFileInfo { FileInfo prefix; int unknown24[3]; } info = {};
    memset(&info, 0, sizeof(info));
    info.prefix.name = mTaskDelete.name;
    int status;
    switch (mpDriver->DeleteFile(mTaskDelete.card, &info.prefix)) {
    case 0: status = 0; break;
    case 3: status = 2; break;
    case 4: status = 11; break;
    case 5: status = 5; break;
    case 10: status = 8; break;
    default: status = 1; break;
    }
    mTaskDelete.manager->EndTask(&mTaskDelete);
    if (mTaskDelete.enable) mTaskMsg.Set(15, status);
}
#endif
}
