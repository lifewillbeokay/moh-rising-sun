// Looping MicroTalk unpacker (supmutlf.c): decodes a block in memory through
// Snd::CMTBLKDecf, resetting the decoder and feeding it from the loop start at the
// end of the block. UNPACKINITPARAMS, CMTBLKDecf and MTFSTATE are named by the mangled
// symbols; the state record (UNPACKMTLSTATE, not an original name) has members
// inferred from offsets.
namespace Snd {
/* inferred: the decoder's restartable state */
struct MTFSTATE {
    int field0;
    int field4;
    int field8;
};

class CMTBLKDecf {
public:
    CMTBLKDecf();
    void* operator new(unsigned int);
    void operator delete(void*);
    int Decode(float**, int);
    MTFSTATE GetState();
    void SetState(MTFSTATE*, int);
    void SetCodecVersion(int);
    int Feed(void*, int, int);

private:
    char m_state[3432];
};
}

struct UNPACKINITPARAMS {
    void* data;
    int field4;
    int field8;
    int length;
    int field10;
    int field14;
    int field18;
    int field1c;
    int field20;
    int field24;
    int (*getframe)(void*);
};

struct UNPACKMTLSTATE {
    int (*unpack)(void*, int, void*, void*, int);
    void (*restore)(void*);
    char field8[20];
    Snd::CMTBLKDecf* decoder;
    int position;
    int loopstart;
    int loopend;
    void* loopdata;
};

int SFILTER_unpackgetframemtlf(void* data) {
    return ((UNPACKMTLSTATE*)data)->position;
}

int SFILTER_unpackmtlf(void* data, int count, void*, void* out, int) {
    UNPACKMTLSTATE* state = (UNPACKMTLSTATE*)data;
    float* destination = (float*)out;

    while (count > 0) {
        int decoded = state->decoder->Decode(&destination, count);
        destination += decoded;
        count -= decoded;
        state->position += decoded;
        if (decoded < count) {
            Snd::MTFSTATE decoderstate;
            int length;

            state->position = state->loopstart;
            decoderstate.field4 = 0;
            decoderstate.field0 = 0;
            decoderstate.field8 = 1;
            state->decoder->SetState(&decoderstate, 0);
            length = state->loopend - state->loopstart + 1;
            state->decoder->Feed(state->loopdata, length * 4, length);
        }
    }
    return 1;
}

void SFILTER_unpackmtlfrestore(void* data) {
    if (((UNPACKMTLSTATE*)data)->decoder)
        delete ((UNPACKMTLSTATE*)data)->decoder;
}

void SFILTER_unpackmtlfinit(void* data, UNPACKINITPARAMS* params) {
    UNPACKMTLSTATE* state = (UNPACKMTLSTATE*)data;

    state->unpack = SFILTER_unpackmtlf;
    state->restore = SFILTER_unpackmtlfrestore;
    state->loopdata = (void*)params->field4;
    state->position = 0;
    state->loopstart = params->field10;
    state->loopend = params->field14;
    state->decoder = new Snd::CMTBLKDecf;
    Snd::MTFSTATE decoderstate;

    decoderstate = state->decoder->GetState();
    decoderstate.field8 = 1;
    state->decoder->SetState(&decoderstate, 0);
    state->decoder->Feed(params->data, state->loopstart * 4, state->loopstart);
    params->getframe = SFILTER_unpackgetframemtlf;
}
