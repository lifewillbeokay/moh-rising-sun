// AI-assisted reconstruction of the character operation tree's evaluators; see docs/Animation.md.
// The source file name is unknown.
#include "profile.h"
#include <algorithm>
#include <vector>
#include "CharacterOperation.h"
extern "C" int rand();
extern "C" double fabs(double);
namespace EALA {
namespace Character {

unsigned int AnimationOperation::size_of(int opcodes) {
    return sizeof(AnimationOperation) + _STL::max(1, opcodes - 1) * sizeof(Opcode);
}

// Plays the animation a random number of times from the inclusive range, with the operation's opcodes.
Resource::Pointer<Task> AnimationOperation::Eval(Resource::Pointer<State> &state, unsigned int substate, Task *parent) {
    unsigned int range = maxLoops - minLoops + 1;
    unsigned int loops = minLoops + rand() % range;
    Resource::Pointer<Playback> playback = Playback::Create(state, substate, parent, animation, name.text, 0.0f, loops);
    playback->AssignOpcodes(opcodeCount, opcodes);
    return playback;
}

} // namespace Character
} // namespace EALA

Serialize::Any &operator<<(Serialize::Any &stream, EALA::Character::Opcode &opcode) {
    stream << opcode.frame << opcode.kind << opcode.argument << opcode.field_06;
    return stream;
}

namespace EALA {
namespace Character {

void AnimationOperation::Serialize(Serialize::Any &stream) {
    stream << minLoops << maxLoops << animation;
    stream << name;
    stream << opcodeCount;
    for (unsigned int i = 0; i < opcodeCount; i++)
        stream << opcodes[i];
}

Resource::Pointer<Task> MessageOperation::Eval(Resource::Pointer<State> &state, unsigned int substate, Task *parent) {
    return Message::Create(state, substate, parent);
}

void MessageOperation::Serialize(Serialize::Any &) {}

Resource::Pointer<Task> TransitionOperation::Eval(Resource::Pointer<State> &state, unsigned int substate, Task *parent) {
    return Jump::Create(state, substate, parent, target, blendFrames);
}

void TransitionOperation::Serialize(Serialize::Any &stream) {
    stream << target << blendFrames;
    stream << name;
}

CompoundOperation::~CompoundOperation() {
    delete op;
    op = 0;
    for (unsigned int i = 0; i < (unsigned int)operandCount; i++) {
        delete operands[i];
        operands[i] = 0;
    }
}

unsigned int CompoundOperation::size_of(int operands) {
    return sizeof(CompoundOperation) + (operands - 1) * sizeof(Operation::Operand *);
}

Resource::Pointer<Task> CompoundOperation::Eval(Resource::Pointer<State> &state, unsigned int substate, Task *parent) {
    return op->Eval(state, substate, parent, operandCount, operands);
}

bool CompoundOperation::IsAuthoredTransition(unsigned int fromPose, unsigned int toPose) {
    return op->IsAuthoredTransition(fromPose, toPose);
}

void CompoundOperation::Serialize(Serialize::Any &stream) {
    stream << operandCount;
    stream << (Serializable *&)op;
    for (unsigned int i = 0; i < (unsigned int)operandCount; i++)
        stream << (Serializable *&)operands[i];
}

// Links each task into a sequence, handing message and jump tasks the task before them.
Resource::Pointer<Task> SequenceOperator::Eval(Resource::Pointer<State> &state, unsigned int substate, Task *parent,
                                               int count, Operation::Operand **operands) {
    Resource::Pointer<Sequence> sequence = Sequence::Create(state, substate, parent);
    Task *sequenceTask = sequence;
    Resource::Pointer<Task> previous;
    for (int i = 0; i < count; i++) {
        Resource::Pointer<Task> task = operands[i]->operation->Eval(state, substate, sequenceTask);
        sequence->AddTask(task);
        if (task->AsMessage()) {
            task->AsMessage()->Assign(previous);
            if (previous.IsValid())
                previous->Reparent(task);
        }
        if (task->AsJump()) {
            task->AsJump()->Assign(previous);
            if (previous.IsValid())
                previous->Reparent(task);
        }
        previous = task;
    }
    if (count == 1) {
        previous->Reparent(parent);
        return previous;
    }
    return sequence;
}

void SequenceOperator::Serialize(Serialize::Any &) {}

// Picks an operand at random by weight, or uniformly when no operand has a weight.
Resource::Pointer<Task> RandomOperator::Eval(Resource::Pointer<State> &state, unsigned int substate, Task *parent,
                                             int count, Operation::Operand **operands) {
    unsigned int total = 0;
    for (int i = 0; i < count; i++)
        total += operands[i]->AsWeightedOperand()->weight;
    unsigned int chosen;
    if (total == 0) {
        chosen = rand() % count;
    } else {
        unsigned int roll = rand() % total;
        chosen = 0;
        while (chosen < (unsigned int)count && roll >= operands[chosen]->AsWeightedOperand()->weight) {
            roll -= operands[chosen]->AsWeightedOperand()->weight;
            chosen++;
        }
    }
    return operands[chosen]->operation->Eval(state, substate, parent);
}

void RandomOperator::Serialize(Serialize::Any &) {}

// A client value names the operand; an out-of-range value takes the first.
Resource::Pointer<Task> SelectionOperator::Eval(Resource::Pointer<State> &state, unsigned int substate, Task *parent,
                                                int count, Operation::Operand **operands) {
    unsigned int index = (int)Choreographer::Current()->GetClient()->GetValue(script.text);
    if (index >= (unsigned int)count)
        index = 0;
    return operands[index]->operation->Eval(state, substate, parent);
}

void SelectionOperator::Serialize(Serialize::Any &stream) { stream << script; }

unsigned int MatrixBlendOperator::size_of(int weights) {
    return sizeof(MatrixBlendOperator) + (weights - 1) * sizeof(Weight);
}

} // namespace Character
} // namespace EALA
