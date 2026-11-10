// AI-assisted reconstruction of the serialized operation tree behind a character state; see docs/Animation.md.
// Class and method names come from the symbol table; member names and the helpers marked inferred are not recovered.
// The interface pragma keeps the tables and implicit destructors out of the units that define the methods; the
// image holds those destructors in shared linkonce storage.
#ifndef MOH_CHARACTEROPERATION_H
#define MOH_CHARACTEROPERATION_H
#pragma interface
#include "CharacterTasks.h"
namespace Serialize {
// The stream base. Slot order and method names come from Serialize::In's table.
class Any {
  public:
    virtual ~Any();
    virtual void serialize(void *data, unsigned int size, unsigned int count) = 0;
    virtual int serialize_ptr(void *pointer, void *data) = 0;
    virtual void start(void *data, unsigned int size, unsigned int align) = 0;
    virtual void finish(void *data) = 0;
    virtual int in() const = 0;
    virtual int out() const = 0;
};
} // namespace Serialize
Serialize::Any &operator<<(Serialize::Any &stream, Serializable *&object);
template <class T> inline Serialize::Any &operator<<(Serialize::Any &stream, T &value) {
    stream.serialize(&value, sizeof(T), 1);
    return stream;
}
inline unsigned int RoundUp(const unsigned int &value, const unsigned int &alignment) {
    unsigned int rounded = value - 1;
    rounded += alignment;
    return rounded / alignment * alignment;
}
// Inferred: the arguments of one out-of-line block in the stream, with the size rounded up to the alignment.
struct SerializedBlock {
    void *pointer;
    void *data;
    unsigned int size;
    unsigned int align;
    SerializedBlock(void *pointer, void *data, unsigned int size, unsigned int alignment)
        : pointer(pointer), data(data), size(size) {
        align = alignment;
        this->size = RoundUp(this->size, align);
    }
};
inline Serialize::Any &operator<<(Serialize::Any &stream, EALA::Character::SerializedString &string) {
    stream << string.length;
    SerializedBlock block(&string.text, string.text, string.length + 1, 4);
    if (stream.serialize_ptr(block.pointer, block.data)) {
        stream.start(block.data, block.size, block.align);
        stream.serialize(block.data, 1, block.size);
        stream.finish(block.data);
    }
    return stream;
}
// Inferred: a script's text has no terminator in the stream, and the code shape differs from SerializedBlock's.
struct SerializedPointer {
    void *pointer;
    SerializedPointer(void *pointer) : pointer(pointer) {}
};
struct SerializedScriptBlock : SerializedPointer {
    void *data;
    unsigned int size;
    unsigned int align;
    SerializedScriptBlock(void *pointer, void *data, unsigned int size, unsigned int alignment)
        : SerializedPointer(pointer), data(data), size(size) {
        align = alignment;
        this->size = (this->size - 1 + align) / align * align;
    }
};
inline void WriteScript(Serialize::Any &stream, const SerializedScriptBlock &block) {
    if (stream.serialize_ptr(block.pointer, block.data)) {
        stream.start(block.data, block.size, block.align);
        stream.serialize(block.data, 1, block.size);
        stream.finish(block.data);
    }
}
Serialize::Any &operator<<(Serialize::Any &stream, EALA::Character::Opcode &opcode);
Serialize::Any &operator<<(Serialize::Any &stream, EALA::Character::Substate &substate);
Serialize::Any &operator<<(Serialize::Any &stream, EALA::Character::State &state);
namespace EALA {
namespace Character {
class SimpleOperand;
class WeightedOperand;
class MatrixBlendOperand;
class TurnOperand;
class Operation::Operator : public Serializable {
  public:
    virtual ~Operator() {}
    virtual Resource::Pointer<Task> Eval(Resource::Pointer<State> &state, unsigned int substate, Task *parent, int count,
                                         Operand **operands) = 0;
    virtual bool IsAuthoredTransition(unsigned int fromPose, unsigned int toPose);
};
class Operation::Operand : public Serializable {
  public:
    virtual void Serialize(Serialize::Any &stream);
    virtual ~Operand();
    virtual SimpleOperand *AsSimpleOperand();
    virtual WeightedOperand *AsWeightedOperand();
    virtual MatrixBlendOperand *AsMatrixBlendOperand();
    virtual TurnOperand *AsTurnOperand();
    Operation *operation; // +0x04
};
class AnimationOperation : public Operation {
  public:
    AnimationOperation(const Serializable::NoInit &);
    static unsigned int size_of(int opcodes);
    virtual void Serialize(Serialize::Any &stream);
    virtual unsigned int SizeOf() const;
    virtual const char *ClassName() const;
    virtual Resource::Pointer<Task> Eval(Resource::Pointer<State> &state, unsigned int substate, Task *parent);
    unsigned int minLoops;         // +0x04
    unsigned int maxLoops;         // +0x08
    unsigned int animation;        // +0x0c
    unsigned short opcodeCount;    // +0x10
    SerializedString name;         // +0x14
    Opcode opcodes[1];             // +0x1c; allocated by size_of for the real count.
};
class MessageOperation : public Operation {
  public:
    MessageOperation(const Serializable::NoInit &);
    virtual void Serialize(Serialize::Any &stream);
    virtual unsigned int SizeOf() const;
    virtual const char *ClassName() const;
    virtual Resource::Pointer<Task> Eval(Resource::Pointer<State> &state, unsigned int substate, Task *parent);
};
class TransitionOperation : public Operation {
  public:
    TransitionOperation(const Serializable::NoInit &);
    virtual void Serialize(Serialize::Any &stream);
    virtual unsigned int SizeOf() const;
    virtual const char *ClassName() const;
    virtual Resource::Pointer<Task> Eval(Resource::Pointer<State> &state, unsigned int substate, Task *parent);
    unsigned int target;           // +0x04
    unsigned int blendFrames;      // +0x08
    SerializedString name;         // +0x0c
};
class CompoundOperation : public Operation {
  public:
    CompoundOperation(const Serializable::NoInit &);
    virtual ~CompoundOperation();
    static unsigned int size_of(int operands);
    virtual void Serialize(Serialize::Any &stream);
    virtual unsigned int SizeOf() const;
    virtual const char *ClassName() const;
    virtual Resource::Pointer<Task> Eval(Resource::Pointer<State> &state, unsigned int substate, Task *parent);
    virtual bool IsAuthoredTransition(unsigned int fromPose, unsigned int toPose);
    int operandCount;              // +0x04
    Operation::Operator *op;                // +0x08
    Operation::Operand *operands[1];         // +0x0c; allocated by size_of for the real count.
};
#define MOH_OPERATOR_METHODS(Name)                                                                                     \
    Name(const Serializable::NoInit &);                                                                                \
    virtual void Serialize(Serialize::Any &stream);                                                                    \
    virtual unsigned int SizeOf() const;                                                                               \
    virtual const char *ClassName() const;                                                                             \
    virtual Resource::Pointer<Task> Eval(Resource::Pointer<State> &state, unsigned int substate, Task *parent,         \
                                         int count, Operation::Operand **operands);
class SequenceOperator : public Operation::Operator {
  public:
    MOH_OPERATOR_METHODS(SequenceOperator)
};
class RandomOperator : public Operation::Operator {
  public:
    MOH_OPERATOR_METHODS(RandomOperator)
};
class SelectionOperator : public Operation::Operator {
  public:
    MOH_OPERATOR_METHODS(SelectionOperator)
    SerializedString script;       // +0x04; the name of the client value that picks the operand.
};
class MatrixBlendOperator : public Operation::Operator {
  public:
    MOH_OPERATOR_METHODS(MatrixBlendOperator)
    static unsigned int size_of(int weights);
    SerializedString rowName;      // +0x04
    SerializedString columnName;   // +0x0c
    int weightCount;               // +0x14
    struct Weight {
        float a, b, c;
    } weights[1];                  // +0x18; allocated by size_of for the real count.
};
class LinearBlendOperator : public Operation::Operator {
  public:
    MOH_OPERATOR_METHODS(LinearBlendOperator)
    static unsigned int size_of(int ranges);
    SerializedString name;         // +0x04
    int rangeCount;                // +0x0c
    struct Range {
        float low, high;
    } ranges[1];                   // +0x10; allocated by size_of for the real count.
};
class TurnOperator : public Operation::Operator {
  public:
    MOH_OPERATOR_METHODS(TurnOperator)
    SerializedString name;         // +0x04; the name of the client value holding the wanted angle.
};
class ScriptOperator : public Operation::Operator {
  public:
    MOH_OPERATOR_METHODS(ScriptOperator)
    SerializedString script;       // +0x04
};
class AuthoredTransitionOperator : public Operation::Operator {
  public:
    MOH_OPERATOR_METHODS(AuthoredTransitionOperator)
    virtual bool IsAuthoredTransition(unsigned int fromPose, unsigned int toPose);
    unsigned int fromPose;         // +0x04
    unsigned int toPose;           // +0x08; recorded by IsAuthoredTransition and matched against operand weights.
};
#undef MOH_OPERATOR_METHODS
class SimpleOperand : public Operation::Operand {
  public:
    SimpleOperand(const Serializable::NoInit &);
    virtual void Serialize(Serialize::Any &stream);
    virtual unsigned int SizeOf() const;
    virtual const char *ClassName() const;
    virtual SimpleOperand *AsSimpleOperand();
};
class WeightedOperand : public Operation::Operand {
  public:
    WeightedOperand(const Serializable::NoInit &);
    virtual void Serialize(Serialize::Any &stream);
    virtual unsigned int SizeOf() const;
    virtual const char *ClassName() const;
    virtual WeightedOperand *AsWeightedOperand();
    unsigned int weight;           // +0x08
};
class MatrixBlendOperand : public Operation::Operand {
  public:
    MatrixBlendOperand(const Serializable::NoInit &);
    virtual void Serialize(Serialize::Any &stream);
    virtual unsigned int SizeOf() const;
    virtual const char *ClassName() const;
    virtual MatrixBlendOperand *AsMatrixBlendOperand();
    float x;                       // +0x08
    float y;                       // +0x0c
};
class TurnOperand : public Operation::Operand {
  public:
    TurnOperand(const Serializable::NoInit &);
    virtual void Serialize(Serialize::Any &stream);
    virtual unsigned int SizeOf() const;
    virtual const char *ClassName() const;
    virtual TurnOperand *AsTurnOperand();
    float GetTaskAngle(Resource::Pointer<State> &state, unsigned int substate, Task *parent);
    float GetTaskDuration(Resource::Pointer<State> &state, unsigned int substate, Task *parent);
    float angle;                   // +0x08; a fraction of a full turn.
    int angleKnown;                // +0x0c
    float taskAngle;               // +0x10
};
} // namespace Character
} // namespace EALA
#endif
