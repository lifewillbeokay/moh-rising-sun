// AI-assisted reconstruction of the character blend tasks; see docs/Animation.md.
// The interface pragma keeps their tables out of the units that define their destructors.
#ifndef MOH_CHARACTERBLENDS_H
#define MOH_CHARACTERBLENDS_H
#pragma interface
namespace EALA {
namespace Character {
class MatrixBlendOperator;
class MatrixBlend : public Task {
  public:
    struct SubTaskInfo {
        float field_00, field_04;
        Resource::Pointer<Task> task;
    };
    static Pointer<MatrixBlend> Create(Pointer<State> &state, unsigned int substate, Task *parent, const char *rowName,
                                const char *columnName, MatrixBlendOperator *op, int count, SubTaskInfo *tasks);
    MatrixBlend(Pointer<State> &state, unsigned int substate, Task *parent, const char *rowName, const char *columnName,
                MatrixBlendOperator *op, int count, SubTaskInfo *tasks);
};
// A scratch pose taken from a small pool for the length of a blend.
class PoseBuffer {
  public:
    PoseBuffer();
    ~PoseBuffer();
    float *pose; // +0x00
};
// Blends the two tasks whose ranges of a client value hold the value's current reading.
class LinearBlend : public Task {
  public:
    struct SubTaskInfo {
        float low, high; // The range of the client value this task covers.
        Resource::Pointer<Task> task;
    };
    void *operator new(unsigned int size);
    void operator delete(void *p, unsigned int size);
    static Pointer<LinearBlend> Create(Pointer<State> &state, unsigned int substate, Task *parent, const char *name,
                                       int count, SubTaskInfo *tasks);
    LinearBlend(Pointer<State> &state, unsigned int substate, Task *parent, const char *name, int count,
                SubTaskInfo *tasks);
    virtual ~LinearBlend();
    virtual int Busy();
    virtual void EvalPose(float time, float *pose);
    virtual void Dump(unsigned int depth);
    void PerformBlend(float value, float time, float *pose);
    const char *name;        // +0x34
    int lowIndex;            // +0x38
    int highIndex;           // +0x3c
    unsigned int count;      // +0x40
    SubTaskInfo tasks[4];    // +0x44
};
} // namespace Character
} // namespace EALA
#endif
