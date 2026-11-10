// AI-assisted reconstruction of the memory-card library's GameCube interface: its destructor and the task thread; see docs/Memory.md.
#include "RealmcInterface.h"
namespace Realmc {
GCInterface::~GCInterface() {
    MUTEX_lock(mMutex);
    mExitThread = 1;
    MUTEX_unlock(mMutex);
    THREAD_waitexit(mMCardThread, 0);
    MUTEX_destroy(mMutex);
    FreeMem(mStackBuffer);
    delete mpDriver;
}
void GCInterface::GCMCThread() {
    while (!mExitThread) {
        if (!mpNewTaskMsg && mTaskManager.current) {
            MUTEX_lock(mMutex);
            mTaskMsg.Reset();
            mpNewTaskMsg = 0;
            switch (mTaskManager.current->id) {
            case 1: UpdateTaskTrcStartGame(); break;
            case 2: UpdateTaskTrcCardExists(); break;
            case 3: UpdateTaskTrcGetCardInfo(); break;
            case 5: UpdateTaskTrcSaveFile(); break;
            case 4: UpdateTaskTrcLoadFile(); break;
            case 6: UpdateTaskTrcDeleteFile(); break;
            case 7: UpdateTaskTrcListFiles(); break;
            case 8: UpdateTaskTrcMountCard(); break;
            case 9: UpdateTaskTrcMountCardOperation(); break;
            case 29: UpdateTaskTrcCheckSpace(); break;
            case 10: UpdateTaskCardExists(); break;
            case 11: UpdateTaskGetCardInfo(); break;
            case 12: UpdateTaskMount(); break;
            case 13: UpdateTaskUnmount(); break;
            case 16: UpdateTaskOpen(); break;
            case 17: UpdateTaskClose(); break;
            case 20: UpdateTaskRead(); break;
            case 21: UpdateTaskWrite(); break;
            case 19: UpdateTaskSeek(); break;
            case 22: UpdateTaskFlush(); break;
            case 18: UpdateTaskDelete(); break;
            case 23: UpdateTaskSetAttribute(); break;
            case 25: UpdateTaskFindFile(); break;
            }
            if (mTaskMsg.id)
                mpNewTaskMsg = &mTaskMsg;
            MUTEX_unlock(mMutex);
        }
        THREAD_yield(5);
    }
}
} // namespace Realmc
