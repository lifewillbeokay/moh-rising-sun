// MicroTalk unpacker (supmutf.c): decodes a block in memory through Snd::CMTBLKDecf,
// padding with silence after its end. UNPACKINITPARAMS, CMTBLKDecf and MTFSTATE are
// named by the mangled symbols; the state record (UNPACKMTSTATE, not an original
// name) has members inferred from offsets.
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

struct UNPACKMTSTATE {
    int (*unpack)(void*, int, void*, void*, int);
    void (*restore)(void*);
    char field8[20];
    unsigned int length;
    unsigned int position;
    int mode;
    Snd::CMTBLKDecf* decoder;
};

extern "C" void* memset(void*, int, unsigned long);

int SFILTER_unpackgetframemtf(void* data) {
    return ((UNPACKMTSTATE*)data)->position;
}

int SFILTER_unpackmtf(void* data, int count, void*, void* out, int) {
    UNPACKMTSTATE* state = (UNPACKMTSTATE*)data;
    float* destination = (float*)out;
    int decoded;

    if (state->position >= state->length)
        return -1;
    decoded = state->decoder->Decode(&destination, count);
    state->position += decoded;
    destination += decoded;
    if (decoded < count) {
        count -= decoded;
        memset(destination, 0, count * 4);
    }
    return 1;
}

void SFILTER_unpackmtfrestore(void* data) {
    if (((UNPACKMTSTATE*)data)->decoder)
        delete ((UNPACKMTSTATE*)data)->decoder;
}

void SFILTER_unpackmtfinit(void* data, UNPACKINITPARAMS* params) {
    UNPACKMTSTATE* state = (UNPACKMTSTATE*)data;

    state->unpack = SFILTER_unpackmtf;
    state->length = params->length;
    state->position = 0;
    state->mode = params->field24;
    params->getframe = SFILTER_unpackgetframemtf;
    state->restore = SFILTER_unpackmtfrestore;
    state->decoder = new Snd::CMTBLKDecf;
    Snd::MTFSTATE decoderstate;

    decoderstate = state->decoder->GetState();
    decoderstate.field8 = 1;
    state->decoder->SetState(&decoderstate, 0);
    if (state->mode <= 2)
        state->decoder->SetCodecVersion(0);
    else
        state->decoder->SetCodecVersion(1);
    state->decoder->Feed(params->data, state->length * 4, state->length);
}
