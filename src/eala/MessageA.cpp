// AI-assisted reconstruction of source/Character/Message.cpp; see docs/Animation.md.
#include "profile.h"
#include "CharacterTasks.h"
namespace EALA {
namespace Character {

Resource::Pointer<Message> Message::Create(Pointer<State> &state, unsigned int substate, Task *parent) {
    return new Message(state, substate, parent);
}

Message::Message(Pointer<State> &state, unsigned int substate, Task *parent)
    : Task(state, substate, parent), sent(0), poseAge(0.0f) {}

void Message::Assign(Pointer<Task> &task) { kept = task; }

int Message::Busy() { return 1; }

void Message::EvalPose(float, float *pose) {
    if (!sent) {
        sent = 1;
        parent->HandleMessage();
    }
    if (pose) {
        if (!(poseAge > 10.0f) && kept.IsValid())
            kept->Evaluate(0.0f, pose);
        else
            Choreographer::Current()->GetClient()->GetSkeleton()->GetStillPose(pose);
    }
}

void Message::Dump(unsigned int depth) {
    DumpPrefix(depth);
    DebugMsg("Message\n");
    if (kept.IsValid())
        kept->Dump(depth + 1);
}

} // namespace Character
} // namespace EALA
