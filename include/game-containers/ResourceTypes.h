// Scoped resource-prefix and container-operation models. See docs/GameAlgorithms.md.
// Pointees remain incomplete. Pointer operations view only the observed Resource prefix.
// Pointer<Resource> models an unknown managed pointee in Context/SubTaskInfo;
// it does not assert the original member specialization or the full pointee layout.
#ifndef MOH_RESOURCE_CONTAINER_TYPES_H
#define MOH_RESOURCE_CONTAINER_TYPES_H
#include <string>
#include <map>
#include "EALAResource.h"
extern "C" int strcasecmp(const char *, const char *);
struct COORD3;
namespace EAGLAnim {
class BoneMask;
}
namespace EALA {
class EAGLLoader;
class Task {
  public:
    virtual void Execute() = 0;
};
namespace Character {
class State;
class Skeleton;
class Opcode {
  public:
    class Handler {
      public:
        virtual int Execute(Opcode &) = 0;
    };
};
class Skeleton {
  public:
    class Partition {
      public:
        unsigned char unresolved_00[36] __attribute__((aligned(4)));
        int field_24;
        static Partition *GetCurrent();
        int operator==(const Partition &other) const;
        const EAGLAnim::BoneMask *GetMask() const;
    };
    unsigned char unresolved_00[4] __attribute__((aligned(4)));
    Partition wholeBody; // +0x04; name inferred, GetBoneMask has no mask for it.
};
class ChoreographerClient {
  public:
    virtual void MoveTo(const COORD3 &position, float time);
    virtual const Skeleton *GetSkeleton();
    virtual COORD3 GetPosition();
    virtual float GetRotation();
    virtual void SetPosition(const COORD3 &position);
    virtual void SetRotation(const float &rotation);
    virtual void StartAnimCallback(const float &duration);
    virtual void UpdateAnimCallback(const float &time);
    virtual float GetValue(const char *name);
    virtual int EvaluateScript(const char *script, unsigned int size, const char *name);
    virtual void OnLocomotionEnd();
    virtual void OnCycleEnd();
    virtual bool IsLoopingDisabled() const;
};
class Choreographer : public Resource {
  public:
    class PoseModifier {
      public:
        PoseModifier *next; // +0x00; the table pointer follows it.
        virtual ~PoseModifier();
        virtual void ModifyPose(float time, float *pose) = 0;
    };
    Choreographer(ChoreographerClient *client);
    virtual ~Choreographer();
    virtual void EvalPose(float time);
    virtual void SetState(const char *name, EALA::Task *done, float blend, const Skeleton::Partition *partition);
    virtual void SetAuthoredTransitionState(const char *name);
    virtual void SetAuthoredTransitionState(Resource::Pointer<State> &state);
    virtual Resource::Pointer<State> GetAuthoredTransitionState();
    virtual ChoreographerClient *GetClient();
    virtual void SetOpcodeHandler(Opcode::Handler *handler);
    virtual void AddPoseModifier(PoseModifier *modifier);
    virtual const EAGLAnim::BoneMask *GetBoneMask();
    virtual void Enable(Skeleton::Partition *partition);
    virtual void Disable(Skeleton::Partition *partition);
    void ApplyMotion();
    static Choreographer *Current();
    void HandleMessage();
    void HandleOpcode(Opcode &opcode);
    void HandleRotation(float rotation);
    void HandleCycleEnd();
    void HandleAnimDuration(float duration);
    struct Context {
        Resource::Pointer<Resource> field_00;
        EALA::Task *done;     // +0x04; run when the context's task sends a message.
        unsigned int field_08; // Set and cleared by Enable and Disable.
        float field_0c;
    };
    struct PartitionCompare {
        bool operator()(const Skeleton::Partition *a, const Skeleton::Partition *b) const {
            int av = 0;
            if (a)
                av = a->field_24;
            int bv = 0;
            if (b)
                bv = b->field_24;
            if (av < bv)
                return true;
            if (bv < av)
                return false;
            return a < b;
        }
    };
    typedef _STL::map<const Skeleton::Partition *, Context, PartitionCompare> ContextMap;

  private:
    unsigned int unknown_08;                 // +0x08; never set by the constructor.
    ChoreographerClient *client;             // +0x0c
    Opcode::Handler *opcodeHandler;          // +0x10
    unsigned int unknown_14;                 // +0x14
    PoseModifier *poseModifiers;             // +0x18; a list linked through each modifier's first word.
    Resource::Pointer<State> authoredState;  // +0x1c
    ContextMap contexts;                     // +0x20
    Resource::Pointer<Resource> current;     // +0x30; compared with each context's task.
};
class MatrixBlend {
  public:
    struct SubTaskInfo {
        float field_00, field_04;
        Resource::Pointer<Resource> field_08;
    };
};
class LinearBlend {
  public:
    struct SubTaskInfo {
        float field_00, field_04;
        Resource::Pointer<Resource> field_08;
    };
};
} // namespace Character
} // namespace EALA
struct StringCompare {
    bool operator()(const _STL::string &a, const _STL::string &b) const {
        return strcasecmp(a.c_str(), b.c_str()) < 0;
    }
};

typedef char ResourcePrefixCheck[sizeof(EALA::Resource) == 8 ? 1 : -1];
typedef char
    ChoreographerContextCheck[sizeof(EALA::Character::Choreographer::Context) == 16 ? 1 : -1];
typedef char MatrixSubTaskCheck[sizeof(EALA::Character::MatrixBlend::SubTaskInfo) == 12 ? 1 : -1];
typedef char LinearSubTaskCheck[sizeof(EALA::Character::LinearBlend::SubTaskInfo) == 12 ? 1 : -1];
#endif
