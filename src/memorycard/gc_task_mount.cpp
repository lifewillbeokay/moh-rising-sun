// AI-assisted reconstruction; see docs/research/card-tasks.md.
#include "RealmcTaskUpdate.h"
namespace Realmc {
#ifdef DECOMP_COMPARE
void GCInterface::UpdateTaskMount() {
    mTaskMount.result = 1;
    switch (mpDriver->Mount(mTaskMount.card)) {
    case 0: mTaskMount.result = 0; break;
    case 2: mTaskMount.result = 6; break;
    case 3: mTaskMount.result = 2; break;
    case 6: case 13: mTaskMount.result = 4; break;
    case 16: mTaskMount.result = 5; break;
    default: mTaskMount.result = 1; break;
    }
    mTaskMount.manager->EndTask(&mTaskMount);
    if (mTaskMount.enable) mTaskMsg.Set(21, mTaskMount.result);
}
#endif
}
