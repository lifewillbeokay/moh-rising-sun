// AI-assisted reconstruction; see docs/research/card-tasks.md.
#include "RealmcTaskUpdate.h"
namespace Realmc {
#ifdef DECOMP_COMPARE
void GCInterface::UpdateTaskTrcGetCardInfo() {
    switch (mTaskTrcGetCardInfo.state) {
    case 0:
        mTaskTrcGetCardInfo.mounted = 0;
        // Fall through to mount.
    case 99:
        mTaskTrcGetCardInfo.state = 103;
        mTaskTrcGetCardInfo.step = 0;
        mTaskTrcMount.Start(mTaskTrcGetCardInfo.card, mTaskTrcGetCardInfo.id);
        break;
    case 103:
        mTaskTrcGetCardInfo.result = mTaskTrcMount.result;
        if (mTaskTrcMount.result != 0 && mTaskTrcMount.result != 3) {
            mTaskTrcGetCardInfo.state = 122;
            mTaskTrcGetCardInfo.step = 0;
            mTaskTrcGetCardInfo.mounted = 0;
            break;
        }
        mTaskTrcGetCardInfo.mounted = 1;
        mTaskTrcGetCardInfo.state = 21;
        mTaskTrcGetCardInfo.step = 0;
        // Fall through to query.
    case 21:
        if (!mpDriver->GetFreeCardSpace(mTaskTrcGetCardInfo.card,
                &mTaskTrcGetCardInfo.freeBytes, &mTaskTrcGetCardInfo.freeFiles))
            mTaskTrcGetCardInfo.result = 0;
        mTaskTrcGetCardInfo.state = 122;
        mTaskTrcGetCardInfo.step = 0;
        // Fall through to completion.
    case 122:
        if (mTaskTrcGetCardInfo.mounted) {
            mpDriver->Unmount(mTaskTrcGetCardInfo.card);
            mTaskTrcGetCardInfo.mounted = 0;
        }
        mTaskMsg.Set(3, mTaskTrcGetCardInfo.result);
        mTaskMsg.card = mTaskTrcGetCardInfo.infoCard;
        mTaskMsg.freeBytes = mTaskTrcGetCardInfo.freeBytes;
        mTaskMsg.freeFiles = mTaskTrcGetCardInfo.freeFiles;
        mTaskTrcGetCardInfo.manager->EndTask(&mTaskTrcGetCardInfo);
        break;
    }
}
#endif
}
