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
};
}
}

void CSoldierObject::EnableSkeletalPartition(Partition partition) {
    switch (partition) {
    case Partition3:
        ((EALA::Character::Choreographer *)((CAnimObject *)this)->choreographer)->Enable((EALA::Character::Skeleton::Partition *)m_partitionLower);
        break;
    case Partition2:
        ((EALA::Character::Choreographer *)((CAnimObject *)this)->choreographer)->Enable((EALA::Character::Skeleton::Partition *)m_partitionUpper);
        break;
    }
}

void CSoldierObject::DisableSkeletalPartition(Partition partition) {
    switch (partition) {
    case Partition3:
        ((EALA::Character::Choreographer *)((CAnimObject *)this)->choreographer)->Disable((EALA::Character::Skeleton::Partition *)m_partitionLower);
        break;
    case Partition2:
        ((EALA::Character::Choreographer *)((CAnimObject *)this)->choreographer)->Disable((EALA::Character::Skeleton::Partition *)m_partitionUpper);
        break;
    }
}
