#include "RealmcTaskManager.h"
namespace Realmc {
void TaskManager::StartTask(Task *task) {
    TrcSingletonAssert(task);
    if (current)
        stack.Push(current);
    current = task;
    task->status = 0;
    task->unknown0c = 0;
    if (IsPublicTrcTask(task))
        publicTasks++;
}
}
