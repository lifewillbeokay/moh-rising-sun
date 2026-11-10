// AI-assisted reconstruction of the EA-XA decoder's allocation and feed; see docs/Sound.md.
#include "SndEAXA.h"
namespace Snd {
void *CEAXABLKDecf::operator new(unsigned int size) {
    return CODANew(size);
}

void CEAXABLKDecf::operator delete(void *p) {
    CODADelete(p);
}

CEAXABLKDecf::CEAXABLKDecf() {
    bytes = 0;
    frames = 0;
    packet.state.previous0 = 0.0f;
    buffered = 0;
    packet.state.previous1 = 0.0f;
}

int CEAXABLKDecf::Feed(void *data, int size, int count) {
    if (!data)
        return -1;
    if (frames)
        return -1;
    frames = count;
    bytes = size;
    packet.data = (unsigned char *)data;
    return 0;
}
}
