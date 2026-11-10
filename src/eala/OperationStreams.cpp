// AI-assisted reconstruction of the character operation tree's streams and blend, turn and script operators; see docs/Animation.md.
// The source file name is unknown.
#include "profile.h"
#include <algorithm>
#include <vector>
#include "CharacterOperation.h"
extern "C" int rand();
extern "C" double fabs(double);
namespace EALA {
namespace Character {

void MatrixBlendOperator::Serialize(Serialize::Any &stream) {
    stream << rowName;
    stream << columnName;
    stream << weightCount;
    for (unsigned int i = 0; i < (unsigned int)weightCount; i++) {
        stream << weights[i].a;
        stream << weights[i].b;
        stream << weights[i].c;
    }
}

unsigned int LinearBlendOperator::size_of(int ranges) {
    return sizeof(LinearBlendOperator) + (ranges - 1) * sizeof(Range);
}

Resource::Pointer<Task> LinearBlendOperator::Eval(Resource::Pointer<State> &state, unsigned int substate, Task *parent,
                                                  int count, Operation::Operand **operands) {
    _STL::vector<LinearBlend::SubTaskInfo> tasks;
    tasks.reserve(count);
    for (int i = 0; i < count; i++) {
        LinearBlend::SubTaskInfo info;
        info.low = ranges[i].low;
        info.high = ranges[i].high;
        info.task = operands[i]->operation->Eval(state, substate, parent);
        tasks.push_back(info);
    }
    return LinearBlend::Create(state, substate, parent, name.text, count, &tasks[0]);
}

void LinearBlendOperator::Serialize(Serialize::Any &stream) {
    stream << name;
    stream << rangeCount;
    for (unsigned int i = 0; i < (unsigned int)rangeCount; i++) {
        stream << ranges[i].low;
        stream << ranges[i].high;
    }
}

// Picks the turn animation closest to the angle in the client's value and scales it to cover the angle.
Resource::Pointer<Task> TurnOperator::Eval(Resource::Pointer<State> &state, unsigned int substate, Task *parent,
                                           int count, Operation::Operand **operands) {
    if (count <= 0)
        return 0;
    float wanted = Choreographer::Current()->GetClient()->GetValue(name.text);
    Operation::Operand *best = operands[0];
    float angle = best->AsTurnOperand()->angle;
    if (!(angle <= 0.5f))
        angle -= 1.0f;
    angle *= 6.2831855f;
    angle = -angle;
    float bestDelta = fabs(angle - wanted);
    for (int i = 1; i < count; i++) {
        angle = operands[i]->AsTurnOperand()->angle;
        if (!(angle <= 0.5f))
            angle -= 1.0f;
        angle *= 6.2831855f;
        angle = -angle;
        float delta = fabs(angle - wanted);
        if (!(delta >= bestDelta)) {
            best = operands[i];
            bestDelta = delta;
        }
    }
    Resource::Pointer<Turn> turn = Turn::Create(state, substate, parent);
    Resource::Pointer<Task> task = best->operation->Eval(state, substate, turn);
    if (task.IsValid()) {
        float from = best->AsTurnOperand()->GetTaskAngle(state, substate, parent);
        float duration = best->AsTurnOperand()->GetTaskDuration(state, substate, parent);
        turn->Assign(task, wanted, from, duration);
    }
    return turn;
}

void TurnOperator::Serialize(Serialize::Any &stream) { stream << name; }

Resource::Pointer<Task> ScriptOperator::Eval(Resource::Pointer<State> &state, unsigned int substate, Task *parent,
                                             int count, Operation::Operand **operands) {
    unsigned int index = Choreographer::Current()->GetClient()->EvaluateScript(script.text, script.length, "(unspecified)");
    if (index >= (unsigned int)count)
        index = 0;
    return operands[index]->operation->Eval(state, substate, parent);
}

void ScriptOperator::Serialize(Serialize::Any &stream) {
    stream << script.length;
    WriteScript(stream, SerializedScriptBlock(&script.text, script.text, script.length, 4));
}

Resource::Pointer<Task> AuthoredTransitionOperator::Eval(Resource::Pointer<State> &state, unsigned int substate,
                                                         Task *parent, int count, Operation::Operand **operands) {
    Resource::Pointer<State> authored = Choreographer::Current()->GetAuthoredTransitionState();
    for (int i = 0; i < count; i++) {
        WeightedOperand *operand = operands[i]->AsWeightedOperand();
        if (operand->weight == toPose) {
            Resource::Pointer<Task> task = operand->operation->Eval(state, substate, parent);
            if (task.IsValid() && task->AsJump())
                task = state->EvalSubstate(task->AsJump()->target, parent);
            return task;
        }
    }
    Resource::Pointer<Task> none(0);
    return none;
}

bool AuthoredTransitionOperator::IsAuthoredTransition(unsigned int fromPose, unsigned int toPose) {
    bool matches;
    if (fromPose != this->fromPose) {
        matches = false;
    } else {
        this->toPose = toPose;
        matches = true;
    }
    return matches;
}

void AuthoredTransitionOperator::Serialize(Serialize::Any &stream) {
    stream << fromPose << toPose;
}

void Operation::Operand::Serialize(Serialize::Any &stream) { stream << (Serializable *&)operation; }

void SimpleOperand::Serialize(Serialize::Any &stream) { Operand::Serialize(stream); }

void WeightedOperand::Serialize(Serialize::Any &stream) {
    Operand::Serialize(stream);
    stream << weight;
}

void MatrixBlendOperand::Serialize(Serialize::Any &stream) {
    Operand::Serialize(stream);
    stream << x << y;
}

float TurnOperand::GetTaskAngle(Resource::Pointer<State> &state, unsigned int substate, Task *parent) {
    if (!angleKnown) {
        if (operation) {
            Resource::Pointer<Task> task = operation->Eval(state, substate, parent);
            if (task.IsValid())
                taskAngle = task->GetTotalRootZAxisRotation();
        }
        angleKnown = 1;
    }
    return taskAngle;
}

} // namespace Character
} // namespace EALA
