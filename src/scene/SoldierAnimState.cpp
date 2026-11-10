// AI-assisted reconstruction from GR8E69. Part of the original Soldier_object.cpp; there is no
// Frontline counterpart, so member names are descriptive. The partition arguments' meaning
// comes from the m_partitionLower and m_partitionUpper symbols.
#include "SceneObjects.h"

namespace EALA {
namespace Character {
class Skeleton {
  public:
    class Partition;
};
class Choreographer {
  public:
    void Enable(Skeleton::Partition *);
    void Disable(Skeleton::Partition *);
    void SetState(const char *, EALA::Task *, float, const Skeleton::Partition *);
};
}
}

int CSoldierObject::SetAnimationState(const char *name, int value, Partition partition) {
    if (!name)
        return 1;
    switch (partition) {
    case Partition3: {
        EALA::Character::Choreographer *choreographer = (EALA::Character::Choreographer *)((CAnimObject *)this)->choreographer;
        lowerState.value = value;
        lowerState.owner = this;
        choreographer->SetState(name, (EALA::Task *)&lowerState, 0.25f, (EALA::Character::Skeleton::Partition *)m_partitionLower);
        choreographer->Enable((EALA::Character::Skeleton::Partition *)m_partitionLower);
        break;
    }
    case Partition2: {
        EALA::Character::Choreographer *choreographer = (EALA::Character::Choreographer *)((CAnimObject *)this)->choreographer;
        upperState.value = value;
        upperState.owner = this;
        choreographer->SetState(name, (EALA::Task *)&upperState, 0.25f, (EALA::Character::Skeleton::Partition *)m_partitionUpper);
        choreographer->Enable((EALA::Character::Skeleton::Partition *)m_partitionUpper);
        break;
    }
    default:
        ((CAnimObject *)this)->stateValue = value;
        ((CAnimObject *)this)->stateOwner = (CAnimObject *)this;
        ((EALA::Character::Choreographer *)((CAnimObject *)this)->choreographer)->SetState(name, (EALA::Task *)&((CAnimObject *)this)->stateTask, 0.25f, 0);
        break;
    }
    return 0;
}

int CSoldierObject::SetAnimationState(const char *name, int value) {
    EALA::Character::Choreographer *choreographer = (EALA::Character::Choreographer *)((CAnimObject *)this)->choreographer;
    choreographer->Disable((EALA::Character::Skeleton::Partition *)m_partitionLower);
    choreographer->Disable((EALA::Character::Skeleton::Partition *)m_partitionUpper);
    return ((CAnimObject *)this)->CAnimObject::SetAnimationState(name, value);
}

int CSoldierObject::SetAnimationState(const char *name, EALA::Task *task) {
    EALA::Character::Choreographer *choreographer = (EALA::Character::Choreographer *)((CAnimObject *)this)->choreographer;
    choreographer->Disable((EALA::Character::Skeleton::Partition *)m_partitionLower);
    choreographer->Disable((EALA::Character::Skeleton::Partition *)m_partitionUpper);
    return ((CAnimObject *)this)->CAnimObject::SetAnimationState(name, task);
}
