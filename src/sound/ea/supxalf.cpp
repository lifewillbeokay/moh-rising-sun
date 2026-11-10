// A fragment of the EA-XA looping unpacker (supxalf.c): the frame getter, the
// restore function and the init, which records the loop bounds and, for mode 3
// and up, feeds the decoder its loop now. SFILTER_unpackxalf, which comes first
// in the original file, is not part of this unit. UNPACKINITPARAMS and
// CEAXABLKDecf are named by the mangled symbols; the state record and members
// are inferred from offsets.
namespace Snd {
/* inferred: the decoder's prediction state */
struct XAFSTATE {
    float previous;
    float current;
};

class CEAXABLKDecf {
public:
    CEAXABLKDecf();
    void* operator new(unsigned int);
    void operator delete(void*);
    int Decode(float**, int);
    XAFSTATE GetState();
    void SetState(XAFSTATE*);
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
    int loopstart;
    int loopend;
    int field18[3];
    int field24;
    int (*getframe)(void*);
};
struct UNPACKXALOOPSTATE {
    int (*unpack)(void*, int, void*, void*, int);
    void (*restore)(void*);
    char field8[20];
    Snd::CEAXABLKDecf* decoder;
    int position;
    int loopstart;
    int loopend;
    int field2C;
    int mode;
    void* data;
    int length;
    int field3C;
    int field40;
    int restartstart;
    int restartend;
    Snd::XAFSTATE saved;
};

// SFILTER_unpackxalf (the looping decode step) comes first in the original
// file and is not reconstructed, so this unit starts after it.
int SFILTER_unpackxalf(void*, int, void*, void*, int);

int SFILTER_unpackgetframexalf(void* data) {
    return ((UNPACKXALOOPSTATE*)data)->position;
}

void SFILTER_unpackxalfrestore(void* data) {
    if (((UNPACKXALOOPSTATE*)data)->decoder)
        delete ((UNPACKXALOOPSTATE*)data)->decoder;
}

void SFILTER_unpackxalfinit(void* data, UNPACKINITPARAMS* params) {
    UNPACKXALOOPSTATE* state = (UNPACKXALOOPSTATE*)data;

    state->unpack = SFILTER_unpackxalf;
    state->restore = SFILTER_unpackxalfrestore;
    state->field2C = params->field4;
    int zero = 0;
    state->position = zero;
    state->mode = params->field24;
    state->loopstart = params->loopstart;
    state->loopend = params->loopend;
    state->decoder = new Snd::CEAXABLKDecf;
    params->getframe = SFILTER_unpackgetframexalf;
    if (state->mode <= 2) {
        state->data = params->data;
        state->length = params->length;
        state->field3C = zero;
        state->field40 = zero;
        state->restartstart = params->loopstart;
        state->restartend = params->loopend;
    } else {
        state->decoder->Feed(params->data, state->loopstart * 4, state->loopstart);
    }
}
