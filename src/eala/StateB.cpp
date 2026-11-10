// AI-assisted reconstruction of source/Character/State.cpp; see docs/Animation.md.
#include "profile.h"
#include "CharacterTasks.h"
extern "C" int printf(const char *, ...);
namespace EALA {
namespace Character {

Resource::Pointer<Animation> State::Cache(unsigned int index) {
    Pointer<Animation> animation = Animation::Alloc(index, bank);
    return animation;
}

Resource::Pointer<Task> State::EvalSubstate(unsigned int substate, Task *parent) {
    Operation *operation = substates[substate]->operation;
    Pointer<State> self(this);
    Pointer<Task> task = operation->Eval(self, substate, parent);
    for (unsigned int jumps = 0; task.IsValid() && task->AsJump() && jumps < 10; jumps++) {
        operation = substates[task->AsJump()->target]->operation;
        task = operation->Eval(self, task->AsJump()->target, parent);
    }
    return task;
}

unsigned int State::GetSubstateIndex(const char *name) {
    if (!name || !*name)
        return 0;
    _STL::string key(name);
    NameMap::iterator found = names->find(key);
    if (found == names->end()) {
        for (found = names->begin(); found != names->end(); ++found)
            printf("substate: %s\n", (*found).first.c_str());
        DebugMsg("Could not find substate: %s.\n", name);
        return 0;
    }
    return (*found).second;
}

Resource::Pointer<Task> State::FindAuthoredTransition(Task *task, unsigned int fromPose, unsigned int toPose) {
    for (unsigned int i = 0; i < substateCount; i++) {
        Operation *operation = substates[i]->operation;
        if (operation->IsAuthoredTransition(fromPose, toPose)) {
            Pointer<State> self(this);
            return operation->Eval(self, toPose, task);
        }
    }
    return 0;
}

} // namespace Character
} // namespace EALA
