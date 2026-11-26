// AI-assisted reconstruction; see docs/research/card-tasks.md.
#include "RealmcTaskUpdate.h"
namespace Realmc {
#ifdef DECOMP_COMPARE
void GCInterface::UpdateTaskTrcListFiles() {
    if (!mMsgTimer.Ready()) return;
    GCMessage *found;
    switch (mTaskTrcListFiles.state) {
    case 0:
        mTaskTrcListFiles.state = 99;
        mTaskTrcListFiles.step = 0;
        mTaskTrcListFiles.result = 27;
        mTaskTrcListFiles.mounted = 0;
        // Fall through to mount.
    case 99:
        if (mTaskTrcListFiles.step == 0) {
            mTaskTrcListFiles.step = 103;
            mTaskTrcMount.Start(mTaskTrcListFiles.card, 7);
        } else if (mTaskTrcListFiles.step == 103) {
            mTaskTrcListFiles.result = mTaskTrcMount.result;
            mTaskTrcListFiles.step = 0;
            if (mTaskTrcMount.result == 0) {
                mTaskTrcListFiles.mounted = 1;
                mTaskTrcListFiles.state = 12;
            } else mTaskTrcListFiles.state = 121;
        }
        break;
    case 12:
        if (mTaskTrcListFiles.step == 0) {
            found = Find(mTaskTrcListFiles.card, mTaskTrcListFiles.info.name, true);
            if (found->result == 11) {
                mTaskTrcListFiles.state = 121;
                mTaskTrcListFiles.step = 0;
                mTaskTrcListFiles.result = 11;
            } else {
                mTaskMsg = *found;
                mTaskTrcListFiles.step = 11;
            }
        } else if (mTaskTrcListFiles.step == 11) {
            found = Find(mTaskTrcListFiles.card, mTaskTrcListFiles.info.name, false);
            if (found->result == 11) {
                mTaskTrcListFiles.state = 121;
                mTaskTrcListFiles.step = 0;
            } else mTaskMsg = *found;
        }
        break;
    case 121:
        if (mTaskTrcListFiles.mounted) {
            mpDriver->Unmount(mTaskTrcListFiles.card);
            mTaskTrcListFiles.mounted = 0;
        }
        mTaskMsg.Set(7, mTaskTrcListFiles.result);
        mTaskTrcListFiles.manager->EndTask(&mTaskTrcListFiles);
        break;
    }
}
#endif
}
