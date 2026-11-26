// AI-assisted reconstruction; see docs/research/card-tasks.md.
#include "RealmcPublic.h"
namespace Realmc {
#ifdef DECOMP_COMPARE
void GCInterface::TrcCardExists(const CardID &card) {
    MUTEX_lock(mMutex);
    mTaskTrcCardExists.Start();
    mTaskTrcCardExists.Card() = card;
    mTaskTrcCardExists.manager->StartTask(&mTaskTrcCardExists);
    MUTEX_unlock(mMutex);
}
void GCInterface::TrcGetCardInfo(const CardID &card) {
    MUTEX_lock(mMutex);
    mTaskTrcGetCardInfo.Start();
    mTaskTrcGetCardInfo.Card() = card;
    memset(&mTaskTrcGetCardInfo.card, 0, 12);
    mTaskTrcGetCardInfo.card = card;
    mTaskTrcGetCardInfo.manager->StartTask(&mTaskTrcGetCardInfo);
    MUTEX_unlock(mMutex);
}
#endif
}
