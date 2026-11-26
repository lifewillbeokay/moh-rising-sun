// AI-assisted reconstruction; see docs/research/card-tasks.md.
#include "RealmcTaskManager.h"
namespace Realmc {
#ifdef DECOMP_COMPARE
void TaskManager::EndTask(Task *task) {
    current = stack.Pop();
    if (IsPublicTrcTask(task)) publicTasks--;
}
#endif
}
