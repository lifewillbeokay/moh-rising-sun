// AI-assisted view of EA's MicroTalk block decoder to float samples (mtdecf.cpp); see docs/Sound.md.
// CMTBLKDecf, UTALKSTATE_CODA and MTFSTATE are named by the mangled symbols; members are inferred.
#ifndef GAME_SND_MICROTALK_H
#define GAME_SND_MICROTALK_H
namespace Snd {
extern void *(*CODANew)(unsigned int);
extern void (*CODADelete)(void *);

struct UTALKSTATE_CODA {
    unsigned char *ptr;
    unsigned int bits;
    int count;
    int firstBit; // +0xc
    int shift;
    float table[64];
    float a[12];
    float b[12];
    float c[324];
    float samples[432];
};

struct MTFSTATE {
    unsigned int bits;
    int available;
    int version;
};

class CMTBLKDecf : public UTALKSTATE_CODA {
  public:
    CMTBLKDecf();
    void *operator new(unsigned int);
    void operator delete(void *);
    int Feed(void *, int, int);
    int Decode(float **, int);
    MTFSTATE GetState();
    void SetState(MTFSTATE *, int);
    void SetCodecVersion(int);
    void initmut(unsigned char *, UTALKSTATE_CODA *, int);

    int available;        // +0xd44
    unsigned char *data;  // +0xd48
    float *out;           // +0xd4c
    short headerSamples;  // +0xd50
    short headerOffset;   // +0xd52
    int frames;           // +0xd54
    int bytes;            // +0xd58
    int version;          // +0xd5c
    int hasHeader;        // +0xd60
    int codecVersion;     // +0xd64
};

void discardbits(UTALKSTATE_CODA *, int);
void readsamples(UTALKSTATE_CODA *, int, float *, int);
void decodemut(UTALKSTATE_CODA *);
} // namespace Snd
#endif
