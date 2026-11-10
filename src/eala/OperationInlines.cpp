// AI-assisted reconstruction of the operation tree's small virtuals; see docs/Animation.md.
// The image places these after the file's static initialization, which is what an inline definition in the class
// would give. Their strings lead the file's .rodata for the same reason. They are out of line here because the class
// tables stay in their implementation unit. The source file name is unknown.
#include "profile.h"
#include "CharacterOperation.h"
namespace EALA {
namespace Character {

Operation::Operand::~Operand() {
    delete operation;
    operation = 0;
}

SimpleOperand *Operation::Operand::AsSimpleOperand() { return 0; }
WeightedOperand *Operation::Operand::AsWeightedOperand() { return 0; }
MatrixBlendOperand *Operation::Operand::AsMatrixBlendOperand() { return 0; }
TurnOperand *Operation::Operand::AsTurnOperand() { return 0; }

AnimationOperation::AnimationOperation(const Serializable::NoInit &) {}
unsigned int AnimationOperation::SizeOf() const { return size_of(opcodeCount); }
const char *AnimationOperation::ClassName() const { return "EALA::Character::AnimationOperation"; }

MessageOperation::MessageOperation(const Serializable::NoInit &) {}
unsigned int MessageOperation::SizeOf() const { return sizeof(MessageOperation); }
const char *MessageOperation::ClassName() const { return "EALA::Character::MessageOperation"; }

TransitionOperation::TransitionOperation(const Serializable::NoInit &) {}
unsigned int TransitionOperation::SizeOf() const { return sizeof(TransitionOperation); }
const char *TransitionOperation::ClassName() const { return "EALA::Character::TransitionOperation"; }

CompoundOperation::CompoundOperation(const Serializable::NoInit &) {}
unsigned int CompoundOperation::SizeOf() const { return size_of(operandCount); }
const char *CompoundOperation::ClassName() const { return "EALA::Character::CompoundOperation"; }

SequenceOperator::SequenceOperator(const Serializable::NoInit &) {}
unsigned int SequenceOperator::SizeOf() const { return sizeof(SequenceOperator); }
const char *SequenceOperator::ClassName() const { return "EALA::Character::SequenceOperator"; }

RandomOperator::RandomOperator(const Serializable::NoInit &) {}
unsigned int RandomOperator::SizeOf() const { return sizeof(RandomOperator); }
const char *RandomOperator::ClassName() const { return "EALA::Character::RandomOperator"; }

SelectionOperator::SelectionOperator(const Serializable::NoInit &) {}
unsigned int SelectionOperator::SizeOf() const { return sizeof(SelectionOperator); }
const char *SelectionOperator::ClassName() const { return "EALA::Character::SelectionOperator"; }

MatrixBlendOperator::MatrixBlendOperator(const Serializable::NoInit &) {}
unsigned int MatrixBlendOperator::SizeOf() const { return size_of(weightCount); }
const char *MatrixBlendOperator::ClassName() const { return "EALA::Character::MatrixBlendOperator"; }

LinearBlendOperator::LinearBlendOperator(const Serializable::NoInit &) {}
unsigned int LinearBlendOperator::SizeOf() const { return size_of(rangeCount); }
const char *LinearBlendOperator::ClassName() const { return "EALA::Character::LinearBlendOperator"; }

TurnOperator::TurnOperator(const Serializable::NoInit &) {}
unsigned int TurnOperator::SizeOf() const { return sizeof(TurnOperator); }
const char *TurnOperator::ClassName() const { return "EALA::Character::TurnOperator"; }

ScriptOperator::ScriptOperator(const Serializable::NoInit &) {}
unsigned int ScriptOperator::SizeOf() const { return sizeof(ScriptOperator); }
const char *ScriptOperator::ClassName() const { return "EALA::Character::ScriptOperator"; }

AuthoredTransitionOperator::AuthoredTransitionOperator(const Serializable::NoInit &) {}
unsigned int AuthoredTransitionOperator::SizeOf() const { return sizeof(AuthoredTransitionOperator); }
const char *AuthoredTransitionOperator::ClassName() const { return "EALA::Character::AuthoredTransitionOperator"; }

SimpleOperand::SimpleOperand(const Serializable::NoInit &) {}
unsigned int SimpleOperand::SizeOf() const { return sizeof(SimpleOperand); }
const char *SimpleOperand::ClassName() const { return "EALA::Character::SimpleOperand"; }
SimpleOperand *SimpleOperand::AsSimpleOperand() { return this; }

WeightedOperand::WeightedOperand(const Serializable::NoInit &) {}
unsigned int WeightedOperand::SizeOf() const { return sizeof(WeightedOperand); }
const char *WeightedOperand::ClassName() const { return "EALA::Character::WeightedOperand"; }
WeightedOperand *WeightedOperand::AsWeightedOperand() { return this; }

MatrixBlendOperand::MatrixBlendOperand(const Serializable::NoInit &) {}
unsigned int MatrixBlendOperand::SizeOf() const { return sizeof(MatrixBlendOperand); }
const char *MatrixBlendOperand::ClassName() const { return "EALA::Character::MatrixBlendOperand"; }
MatrixBlendOperand *MatrixBlendOperand::AsMatrixBlendOperand() { return this; }

TurnOperand::TurnOperand(const Serializable::NoInit &) {}
unsigned int TurnOperand::SizeOf() const { return sizeof(TurnOperand); }
const char *TurnOperand::ClassName() const { return "EALA::Character::TurnOperand"; }
TurnOperand *TurnOperand::AsTurnOperand() { return this; }

} // namespace Character
} // namespace EALA
