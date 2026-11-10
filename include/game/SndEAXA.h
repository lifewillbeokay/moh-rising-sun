// AI-assisted view of EA's EA-XA block decoder to float samples (eaxadecf.cpp); see docs/Sound.md.
// CEAXABLKDecf, MXAPACKETF and XAFSTATE are named by the mangled symbols; members are inferred.
#ifndef GAME_SND_EAXA_H
#define GAME_SND_EAXA_H
namespace Snd {
extern void *(*CODANew)(unsigned int);
extern void (*CODADelete)(void *);

struct XAFSTATE {
    float previous0;
    float previous1;
};

struct MXAPACKETF {
    int count;
    XAFSTATE state;
    unsigned char *data;
    float *out;
};
void decodexac(MXAPACKETF *);

class CEAXABLKDecf {
  public:
    CEAXABLKDecf();
    void *operator new(unsigned int);
    void operator delete(void *);
    int Feed(void *, int, int);
    int Decode(float **, int);
    XAFSTATE GetState();
    void SetState(XAFSTATE *);

    int field0;
    int buffered;  // +4; samples left in the block buffer.
    int frames;    // +8
    int bytes;     // +0xc
    float *read;   // +0x10; next sample in the block buffer.
    char field14[16];
    float buffer[28]; // +0x24
    MXAPACKETF packet; // +0x94
};
} // namespace Snd
#endif
