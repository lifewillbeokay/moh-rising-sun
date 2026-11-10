// AI-assisted reconstruction of the MicroTalk decoder's speech filter and stream start; see docs/Sound.md.
#include "SndMicroTalk.h"
namespace Snd {
unsigned int getbits(UTALKSTATE_CODA *, int);
static void interpolate(float *p) {
    for (int j = 0; j < 108; j += 2)
        p[j] = 0.59738594f * (p[j - 1] + p[j + 1]) + -0.11459156f * (p[j - 3] + p[j + 3]) + 0.018032679f * (p[j - 5] + p[j + 5]);
}

static void reftolpc(float* k, float* a) {
    float t[12];
    float h[12];
    float x;
    int i;
    int j;

    for (j = 10; j >= 0; j--)
        t[j + 1] = k[j];
    t[0] = 1.0f;
    for (i = 0; i < 12; i++) {
        x = -k[11] * t[11];
        for (j = 10; j >= 0; j--) {
            x -= k[j] * t[j];
            t[j + 1] = k[j] * x + t[j];
        }
        t[0] = x;
        h[i] = x;
        for (j = 0; j < i; j++)
            x -= a[j] * h[i - j - 1];
        a[i] = x;
    }
}

static void filter(UTALKSTATE_CODA * s, int offset, int blocks) {
    float lpc[12];
    float* out = &s->samples[offset];
    int i;

    reftolpc(s->a, lpc);
    for (i = 0; i < blocks; i++) {
        s->b[11] = out[0] + lpc[0] * s->b[0] + lpc[1] * s->b[1] + lpc[2] * s->b[2] + lpc[3] * s->b[3] + lpc[4] * s->b[4] + lpc[5] * s->b[5] + lpc[6] * s->b[6] + lpc[7] * s->b[7] + lpc[8] * s->b[8] + lpc[9] * s->b[9] + lpc[10] * s->b[10] + lpc[11] * s->b[11];
        out[0] = s->b[11];
        s->b[10] = out[1] + lpc[0] * s->b[11] + lpc[1] * s->b[0] + lpc[2] * s->b[1] + lpc[3] * s->b[2] + lpc[4] * s->b[3] + lpc[5] * s->b[4] + lpc[6] * s->b[5] + lpc[7] * s->b[6] + lpc[8] * s->b[7] + lpc[9] * s->b[8] + lpc[10] * s->b[9] + lpc[11] * s->b[10];
        out[1] = s->b[10];
        s->b[9] = out[2] + lpc[0] * s->b[10] + lpc[1] * s->b[11] + lpc[2] * s->b[0] + lpc[3] * s->b[1] + lpc[4] * s->b[2] + lpc[5] * s->b[3] + lpc[6] * s->b[4] + lpc[7] * s->b[5] + lpc[8] * s->b[6] + lpc[9] * s->b[7] + lpc[10] * s->b[8] + lpc[11] * s->b[9];
        out[2] = s->b[9];
        s->b[8] = out[3] + lpc[0] * s->b[9] + lpc[1] * s->b[10] + lpc[2] * s->b[11] + lpc[3] * s->b[0] + lpc[4] * s->b[1] + lpc[5] * s->b[2] + lpc[6] * s->b[3] + lpc[7] * s->b[4] + lpc[8] * s->b[5] + lpc[9] * s->b[6] + lpc[10] * s->b[7] + lpc[11] * s->b[8];
        out[3] = s->b[8];
        s->b[7] = out[4] + lpc[0] * s->b[8] + lpc[1] * s->b[9] + lpc[2] * s->b[10] + lpc[3] * s->b[11] + lpc[4] * s->b[0] + lpc[5] * s->b[1] + lpc[6] * s->b[2] + lpc[7] * s->b[3] + lpc[8] * s->b[4] + lpc[9] * s->b[5] + lpc[10] * s->b[6] + lpc[11] * s->b[7];
        out[4] = s->b[7];
        s->b[6] = out[5] + lpc[0] * s->b[7] + lpc[1] * s->b[8] + lpc[2] * s->b[9] + lpc[3] * s->b[10] + lpc[4] * s->b[11] + lpc[5] * s->b[0] + lpc[6] * s->b[1] + lpc[7] * s->b[2] + lpc[8] * s->b[3] + lpc[9] * s->b[4] + lpc[10] * s->b[5] + lpc[11] * s->b[6];
        out[5] = s->b[6];
        s->b[5] = out[6] + lpc[0] * s->b[6] + lpc[1] * s->b[7] + lpc[2] * s->b[8] + lpc[3] * s->b[9] + lpc[4] * s->b[10] + lpc[5] * s->b[11] + lpc[6] * s->b[0] + lpc[7] * s->b[1] + lpc[8] * s->b[2] + lpc[9] * s->b[3] + lpc[10] * s->b[4] + lpc[11] * s->b[5];
        out[6] = s->b[5];
        s->b[4] = out[7] + lpc[0] * s->b[5] + lpc[1] * s->b[6] + lpc[2] * s->b[7] + lpc[3] * s->b[8] + lpc[4] * s->b[9] + lpc[5] * s->b[10] + lpc[6] * s->b[11] + lpc[7] * s->b[0] + lpc[8] * s->b[1] + lpc[9] * s->b[2] + lpc[10] * s->b[3] + lpc[11] * s->b[4];
        out[7] = s->b[4];
        s->b[3] = out[8] + lpc[0] * s->b[4] + lpc[1] * s->b[5] + lpc[2] * s->b[6] + lpc[3] * s->b[7] + lpc[4] * s->b[8] + lpc[5] * s->b[9] + lpc[6] * s->b[10] + lpc[7] * s->b[11] + lpc[8] * s->b[0] + lpc[9] * s->b[1] + lpc[10] * s->b[2] + lpc[11] * s->b[3];
        out[8] = s->b[3];
        s->b[2] = out[9] + lpc[0] * s->b[3] + lpc[1] * s->b[4] + lpc[2] * s->b[5] + lpc[3] * s->b[6] + lpc[4] * s->b[7] + lpc[5] * s->b[8] + lpc[6] * s->b[9] + lpc[7] * s->b[10] + lpc[8] * s->b[11] + lpc[9] * s->b[0] + lpc[10] * s->b[1] + lpc[11] * s->b[2];
        out[9] = s->b[2];
        s->b[1] = out[10] + lpc[0] * s->b[2] + lpc[1] * s->b[3] + lpc[2] * s->b[4] + lpc[3] * s->b[5] + lpc[4] * s->b[6] + lpc[5] * s->b[7] + lpc[6] * s->b[8] + lpc[7] * s->b[9] + lpc[8] * s->b[10] + lpc[9] * s->b[11] + lpc[10] * s->b[0] + lpc[11] * s->b[1];
        out[10] = s->b[1];
        s->b[0] = out[11] + lpc[0] * s->b[1] + lpc[1] * s->b[2] + lpc[2] * s->b[3] + lpc[3] * s->b[4] + lpc[4] * s->b[5] + lpc[5] * s->b[6] + lpc[6] * s->b[7] + lpc[7] * s->b[8] + lpc[8] * s->b[9] + lpc[9] * s->b[10] + lpc[10] * s->b[11] + lpc[11] * s->b[0];
        out[11] = s->b[0];
        out += 12;
    }
}


void CMTBLKDecf::initmut(unsigned char *source, UTALKSTATE_CODA *s, int version) {
    s->ptr = ++source;
    s->count = 8;
    s->bits = source[-1];
    if (version) {
        s->firstBit = getbits(s, 1);
        s->shift = 32 - getbits(s, 4);
        s->table[0] = 8.0f * (float)(getbits(s, 4) + 1);
        float ratio = 1.04f + 0.001f * (float)getbits(s, 6);
        int i;
        for (i = 1; i < 64; i++)
            s->table[i] = s->table[i - 1] * ratio;
        for (i = 0; i < 12; i++) {
            s->a[i] = 0.0f;
            s->b[i] = 0.0f;
        }
        for (i = 0; i < 324; i++)
            s->c[i] = 0.0f;
    }
}
}
