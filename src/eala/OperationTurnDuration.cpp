// AI-assisted reconstruction of the turn operand's duration query and stream; see docs/Animation.md.
// The source file name is unknown.
#include "profile.h"
#include <algorithm>
#include <vector>
#include "CharacterOperation.h"
extern "C" int rand();
extern "C" double fabs(double);
namespace EALA {
namespace Character {

float TurnOperand::GetTaskDuration(Resource::Pointer<State> &state, unsigned int substate, Task *parent) {
    float duration = 0.0f;
    if (operation) {
        Resource::Pointer<Task> task = operation->Eval(state, substate, parent);
        if (task.IsValid())
            duration = task->GetAnimDuration();
    }
    return duration;
}

void TurnOperand::Serialize(Serialize::Any &stream) {
    Operand::Serialize(stream);
    stream << angle;
}

} // namespace Character
} // namespace EALA
