// AI-assisted reconstruction; see docs/research/card-tasks.md.
#include "RealmcTaskUpdate.h"
namespace Realmc {
void GCInterface::UpdateTaskSetAttribute() {
    int status;
    switch (mpDriver->SetAttributes(mTaskSetAttribute.card, mTaskSetAttribute.name,
            (FileAttribute)mTaskSetAttribute.value)) {
    case 0: status = 0; break;
    case 3: status = 2; break;
    case 4: status = 11; break;
    case 10: status = 8; break;
    default: status = 1; break;
    }
    if (mTaskSetAttribute.enable) mTaskMsg.Set(16, status);
    mTaskSetAttribute.manager->EndTask(&mTaskSetAttribute);
}
}
