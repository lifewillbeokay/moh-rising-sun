// MicroTalk packet unpacker (supmutpf.c): decodes packets from the voice's
// packet player through Snd::CMTBLKDecf. A packet with no flag word restarts the
// decoder state; modes above 2 use codec version 1. UNPACKINITPARAMS, CMTBLKDecf
// and MTFSTATE are named by the mangled symbols; the state record and the members
// are inferred from offsets.
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
    int field10[3];
    int voice;
    int field20;
    int field24;
    int (*getframe)(void*);
};
struct UNPACKMTPACKETSTATE {
    int (*unpack)(void*, int, void*, void*, int);
    void (*restore)(void*);
    char field8[20];
    unsigned char* data;
    int length;
    int packethandle;
    unsigned int consumed;
    unsigned short position;
    int mode;
    Snd::CMTBLKDecf* decoder;
    unsigned char channel;
};

int SNDDRV_getmastervoice(int);
void SNDPKTPLAYI_freeframes(int, int, int);
unsigned char* SNDPKTPLAYI_get(int, int, int*, int*);
extern "C" void* memset(void*, int, unsigned long);
int SNDPKTPLAYI_voicetopackethandle(int);
int SNDDRV_getsamplechan(int);

int SFILTER_unpackmtpf(void* data, int count, void*, void* out, int) {
    UNPACKMTPACKETSTATE* state = (UNPACKMTPACKETSTATE*)data;
    int frames;
    int total = 0;
    float* destination = (float*)out;
    Snd::MTFSTATE decoderstate;

    if (state->consumed) {
        SNDPKTPLAYI_freeframes(state->packethandle, state->channel, state->consumed);
        state->consumed = 0;
    }
    while (count > 0) {
        int decoded = state->decoder->Decode(&destination, count);
        state->position += decoded;
        destination += decoded;
        state->consumed += decoded;
        total += decoded;
        if (decoded < count) {
            count -= decoded;
            state->data = SNDPKTPLAYI_get(state->packethandle, state->channel, &state->length, &frames);
            if (state->data) {
                state->position = 0;
                if (state->mode <= 2) {
                    if (frames == 0) {
                        decoderstate = state->decoder->GetState();
                        decoderstate.field8 = 1;
                        state->decoder->SetState(&decoderstate, 0);
                    } else {
                        decoderstate = state->decoder->GetState();
                        decoderstate.field8 = 0;
                        state->decoder->SetState(&decoderstate, 0);
                    }
                    state->decoder->SetCodecVersion(0);
                } else {
                    int flags = frames;
                    if (flags == 0) {
                        decoderstate = state->decoder->GetState();
                        decoderstate.field4 = flags;
                        decoderstate.field8 = 1;
                    } else {
                        decoderstate = state->decoder->GetState();
                        decoderstate.field8 = 0;
                    }
                    state->decoder->SetState(&decoderstate, 0);
                    state->decoder->SetCodecVersion(1);
                }
                state->decoder->Feed(state->data + 1, state->length * 4, state->length);
            } else {
                if (count > 0 && state->consumed)
                    memset(destination, 0, count * 4);
                state->length = 0;
                count = 0;
            }
        } else {
            count -= decoded;
        }
    }
    return total;
}

void SFILTER_unpackmtpfrestore(void* data) {
    if (((UNPACKMTPACKETSTATE*)data)->decoder)
        delete ((UNPACKMTPACKETSTATE*)data)->decoder;
}

void SFILTER_unpackmtpfinit(void* data, UNPACKINITPARAMS* params) {
    UNPACKMTPACKETSTATE* state = (UNPACKMTPACKETSTATE*)data;

    state->unpack = SFILTER_unpackmtpf;
    int zero = 0;
    state->packethandle = SNDPKTPLAYI_voicetopackethandle(SNDDRV_getmastervoice(params->voice));
    state->channel = SNDDRV_getsamplechan(params->voice);
    state->data = (unsigned char*)zero;
    state->position = zero;
    state->mode = params->field24;
    state->length = params->length;
    state->restore = SFILTER_unpackmtpfrestore;
    state->decoder = new Snd::CMTBLKDecf;
    state->consumed = zero;
}
