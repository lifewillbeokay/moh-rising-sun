// A fragment of the memory-card library's public interface (public.cpp): starting a game's trc checks.
#include "RealmcPublic.h"
namespace Realmc {
void GCInterface::TrcStartGame(const StartGameInfo &info) {
    MUTEX_lock(mMutex);
    TaskTrcStart *task = &mTaskTrcStartGame;
    task->Start();
    task->Card() = info.card;
    task->info = info;
    task->manager->StartTask(task);
    MUTEX_unlock(mMutex);
}
}
