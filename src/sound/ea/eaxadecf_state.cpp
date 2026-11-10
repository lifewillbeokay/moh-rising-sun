// AI-assisted reconstruction of the EA-XA decoder's state access; see docs/Sound.md.
#include "SndEAXA.h"
namespace Snd {
XAFSTATE CEAXABLKDecf::GetState() {
    XAFSTATE state;
    state.previous0 = packet.state.previous0;
    state.previous1 = packet.state.previous1;
    return state;
}

void CEAXABLKDecf::SetState(XAFSTATE *state) {
    packet.state.previous0 = state->previous0;
    packet.state.previous1 = state->previous1;
}
}
