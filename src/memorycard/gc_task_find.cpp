// AI-assisted reconstruction; see docs/research/card-tasks.md.
#include "RealmcTaskUpdate.h"
namespace Realmc {
#ifdef DECOMP_COMPARE
void GCInterface::UpdateTaskFindFile() {
    GCMessage *found;
    switch (mTaskFind.state) {
    case 0:
        mTaskFind.state = 10;
        mTaskFind.step = 0;
        // Fall through to the first search.
    case 10:
        found = Find(mTaskFind.card, mTaskFind.name, true);
        if (found->result == 11) {
            mTaskFind.state = 121;
            mTaskFind.step = 0;
        } else {
            mTaskFind.value++;
            if (mTaskFind.enable) mTaskMsg = *found;
            mTaskFind.state = 11;
            mTaskFind.step = 0;
        }
        break;
    case 11:
        found = Find(mTaskFind.card, mTaskFind.name, false);
        if (found->result == 11) {
            mTaskFind.state = 121;
            mTaskFind.step = 0;
        } else {
            mTaskFind.value++;
            if (mTaskFind.enable) mTaskMsg = *found;
        }
        break;
    case 121:
        mTaskFind.manager->EndTask(&mTaskFind);
        if (mTaskFind.enable) mTaskMsg.Set(12, 0);
        break;
    }
}
#endif
}
