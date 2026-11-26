#ifndef GAME_REALMC_TASK_MANAGER_H
#define GAME_REALMC_TASK_MANAGER_H
// A scoped view of the memory-card library task manager and interrupted-task stack. Class and method names come from the symbol table; member names are inferred from the code. These
// methods are virtual in the original.
namespace Realmc {
class Task {
  public:
    unsigned char unknown00[4];
    int id;     // +0x04
    int status; // +0x08
    int unknown0c;
    virtual void Init(bool); // Table pointer at +0x10.
};

class GcTask : public Task {
  public:
    unsigned short slot;     // +0x14
    int kind;                // +0x18
    int enable;              // +0x1c
};

class TaskTrc : public GcTask {
  public:
    virtual void Clear();
    virtual void Init(bool);
};

// The tasks waiting under the current one; the name is inferred.
struct TaskStack {
    Task *tasks[16];
    int count; // +0x40
    void Push(Task *task) {
        if (count <= 15)
            tasks[count++] = task;
    }
    Task *Pop() {
        return count > 0 ? tasks[--count] : 0;
    }
};

class TaskManager {
  public:
    Task *current;
    TaskStack stack;   // +0x04
    int publicTasks;   // +0x48
    void StartTask(Task *);
    void EndTask(Task *);
    void UpdateCurrentTask();
    int IsPublicTrcTask(Task *);
    void TrcSingletonAssert(Task *);
};

}
#endif
