// AI-assisted reconstruction of source/Character/Choreographer.cpp; see docs/Animation.md.
#include "profile.h"
#include "ResourceTypes.h"
namespace EALA {
namespace Character {

void Choreographer::SetAuthoredTransitionState(Resource::Pointer<State> &state) { authoredState = state; }

Resource::Pointer<State> Choreographer::GetAuthoredTransitionState() { return authoredState; }

void Choreographer::Enable(Skeleton::Partition *partition) {
    ContextMap::iterator found = contexts.find(partition);
    if (found != contexts.end())
        (*found).second.field_08 = 1;
}

void Choreographer::Disable(Skeleton::Partition *partition) {
    ContextMap::iterator found = contexts.find(partition);
    if (found != contexts.end())
        (*found).second.field_08 = 0;
}

void Choreographer::SetOpcodeHandler(Opcode::Handler *handler) { opcodeHandler = handler; }

const EAGLAnim::BoneMask *Choreographer::GetBoneMask() {
    Skeleton::Partition *partition = Skeleton::Partition::GetCurrent();
    if (!partition)
        return 0;
    return *partition == GetClient()->GetSkeleton()->wholeBody ? 0 : partition->GetMask();
}

} // namespace Character
} // namespace EALA
