// AI-assisted reconstruction; see docs/research/card-tasks.md.
#include "RealmcTaskUpdate.h"
namespace Realmc {
#ifdef DECOMP_COMPARE
void GCInterface::UpdateTaskTrcCardExists() {
    switch (mTaskTrcCardExists.state) {
    case 0:
        mTaskTrcCardExists.mounted = 0;
        // Fall through to mount.
    case 99:
        mTaskTrcCardExists.state = 103;
        mTaskTrcCardExists.step = 0;
        mTaskTrcMount.Start(mTaskTrcCardExists.card, mTaskTrcCardExists.id);
        break;
    case 103:
        mTaskTrcCardExists.mounted = mTaskTrcMount.result == 0 || mTaskTrcMount.result == 3;
        mTaskTrcCardExists.result = mTaskTrcMount.result;
        mTaskTrcCardExists.state = 122;
        mTaskTrcCardExists.step = 0;
        // Fall through to completion.
    case 122:
        if (mTaskTrcCardExists.mounted) {
            mpDriver->Unmount(mTaskTrcCardExists.card);
            mTaskTrcCardExists.mounted = 0;
        }
        mTaskMsg.Set(2, mTaskTrcCardExists.result);
        mTaskMsg.card = mTaskTrcCardExists.card;
        mTaskTrcCardExists.manager->EndTask(&mTaskTrcCardExists);
        break;
    }
}
#endif
}
