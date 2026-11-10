// EA-XA unpacker for sound memory (supxaf.c), decoding through
// Snd::CEAXABLKDecf (four-byte float samples; the tail of a short decode is
// zero-filled). UNPACKINITPARAMS and CEAXABLKDecf are named by the mangled
// symbols; the state record (UNPACKXASTATE, not an original name) and the
// members are inferred from offsets.
namespace Snd {
class CEAXABLKDecf {
public:
    CEAXABLKDecf();
    void* operator new(unsigned int);
    void operator delete(void*);
    int Decode(float**, int);
    int Feed(void*, int, int);

private:
    char m_state[168];
};
}

struct UNPACKINITPARAMS {
    void* data;
    int field4;
    int field8;
    int length;
    int field10[6];
    int (*getframe)(void*);
};

struct UNPACKXASTATE {
    int (*unpack)(void*, int, void*, void*, int);
    void (*restore)(void*);
    char field8[20];
    Snd::CEAXABLKDecf* decoder;
    void* data;
    int length;
    int position;
};

extern "C" void* memset(void*, int, unsigned long);

int SFILTER_unpackxaf(void* data, int count, void*, void* out, int) {
    UNPACKXASTATE* state = (UNPACKXASTATE*)data;
    float* destination = (float*)out;
    int decoded;

    if (state->position >= state->length)
        return -1;
    decoded = state->decoder->Decode(&destination, count);
    state->position += decoded;
    destination += decoded;
    if (decoded < count) {
        count -= decoded;
        memset(destination, 0, count * sizeof(float));
        decoded += count;
    }
    return decoded;
}

int SFILTER_unpackgetframexaf(void* data) {
    return ((UNPACKXASTATE*)data)->position;
}

void SFILTER_unpackxafrestore(void* data) {
    if (((UNPACKXASTATE*)data)->decoder)
        delete ((UNPACKXASTATE*)data)->decoder;
}

void SFILTER_unpackxafinit(void* data, UNPACKINITPARAMS* params) {
    UNPACKXASTATE* state = (UNPACKXASTATE*)data;

    state->unpack = SFILTER_unpackxaf;
    state->restore = SFILTER_unpackxafrestore;
    state->data = params->data;
    state->length = params->length;
    state->position = 0;
    state->decoder = new Snd::CEAXABLKDecf;
    params->getframe = SFILTER_unpackgetframexaf;
    state->decoder->Feed(state->data, state->length * 4, state->length);
}
