// AI-assisted reconstruction of the MicroTalk decoder's feed, decode and state; see docs/Sound.md.
#include "SndMicroTalk.h"
extern "C" void *memcpy(void *, const void *, unsigned int);
namespace Snd {
CMTBLKDecf::CMTBLKDecf() {
    available = 0;
    frames = 0;
    bytes = 0;
    headerSamples = 0;
    headerOffset = 0;
    version = 0;
    codecVersion = 1;
    hasHeader = 0;
}

int CMTBLKDecf::Feed(void *source, int size, int count) {
    if (!source || frames)
        return -1;
    frames = count;
    bytes = size;
    available = 0;
    data = (unsigned char *)source;
    if (codecVersion == 1) {
        if (*(unsigned char *)source == 0xee)
            hasHeader = 1;
        else
            hasHeader = 0;
        initmut((unsigned char *)source + 1, this, version);
    } else {
        initmut((unsigned char *)source, this, version);
    }
    return 0;
}

static inline void ReadShort(short &value, const unsigned char *source) {
    ((unsigned char *)&value)[0] = source[0];
    ((unsigned char *)&value)[1] = source[1];
}

int CMTBLKDecf::Decode(float **output, int count) {
    int ready = available;
    if (!frames)
        return 0;
    out = *output;
    int wanted = frames;
    if (wanted > count)
        wanted = count;
    int left = wanted;
    while (left > 0) {
        if (left < ready)
            ready = left;
        int size = ready * sizeof(float);
        memcpy(out, &samples[432 - available], size);
        left -= ready;
        out = (float *)((char *)out + size);
        available -= ready;
        if (left > 0) {
            decodemut(this);
            available = 432;
            ready = 432;
            if (codecVersion == 1) {
                unsigned char *p = data = ptr - 1;
                if (hasHeader) {
                    ReadShort(headerOffset, p);
                    ReadShort(headerSamples, p + 2);
                    unsigned char *next = data + headerSamples * 2;
                    data = next + 4;
                    ptr = next + 5;
                    float *history = &samples[headerOffset];
                    p += 4;
                    for (short i = 0; i < headerSamples; i++) {
                        short value;
                        ReadShort(value, p);
                        p += 2;
                        *history++ = (float)value;
                    }
                    hasHeader = 0;
                }
                if (*data == 0xee)
                    hasHeader = 1;
                this->count = 8;
                bits = *ptr;
                ptr++;
            }
        }
    }
    frames -= wanted;
    return wanted;
}

MTFSTATE CMTBLKDecf::GetState() {
    MTFSTATE state;
    state.bits = bits;
    state.version = version;
    state.available = available;
    return state;
}

void CMTBLKDecf::SetState(MTFSTATE *state, int) {
    bits = state->bits;
    version = state->version;
    available = state->available;
}

void CMTBLKDecf::SetCodecVersion(int value) {
    codecVersion = value;
}
}
