// AI-assisted reconstruction of source/Character/State.cpp's streams; see docs/Animation.md.
#include "profile.h"
#include <new>
#include "CharacterOperation.h"

Serialize::Any &operator<<(Serialize::Any &stream, EALA::Character::Substate &substate) {
    stream << (Serializable *&)substate.operation << substate.name << substate.unknown_00;
    return stream;
}

static inline void SerializeSubstate(Serialize::Any &stream, EALA::Character::Substate *&substate) {
    if (stream.serialize_ptr(&substate, substate)) {
        stream.start(substate, sizeof(EALA::Character::Substate), 4);
        stream << *substate;
        stream.finish(substate);
    }
}

Serialize::Any &operator<<(Serialize::Any &stream, EALA::Character::State &state) {
    if (stream.in())
        new (&state) EALA::Character::State();
    stream << state.version[0] << state.version[1] << state.version[2] << state.version[3];
    stream << state.fileSize;
    if (state.fileSize)
        WriteScript(stream, SerializedScriptBlock(&state.fileData, state.fileData, state.fileSize, 16));
    stream << state.unknown_20 << state.substateCount;
    for (unsigned int i = 0; i < state.substateCount; i++) {
        SerializeSubstate(stream, state.substates[i]);
    }
    return stream;
}
