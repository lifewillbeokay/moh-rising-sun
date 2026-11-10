// AI-assisted reconstruction of source/Character/Choreographer.cpp; see docs/Animation.md.
#include "profile.h"
#include "ResourceTypes.h"
namespace EALA {
namespace Character {

void Choreographer::HandleMessage() {
    ContextMap::iterator it = contexts.begin();
    ContextMap::iterator end = contexts.end();
    for (; it != end; ++it) {
        ContextMap::value_type &entry = *it;
        if ((Resource *)entry.second.field_00 == (Resource *)current && entry.second.done)
            entry.second.done->Execute();
    }
}

void Choreographer::HandleOpcode(Opcode &opcode) {
    if (opcodeHandler)
        opcodeHandler->Execute(opcode);
}

void Choreographer::HandleRotation(float rotation) { GetClient()->SetRotation(rotation); }

void Choreographer::HandleCycleEnd() { GetClient()->OnCycleEnd(); }

void Choreographer::HandleAnimDuration(float duration) { GetClient()->StartAnimCallback(duration); }

ChoreographerClient *Choreographer::GetClient() { return client; }

} // namespace Character
} // namespace EALA
