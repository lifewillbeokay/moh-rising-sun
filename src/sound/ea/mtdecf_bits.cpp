// AI-assisted reconstruction of the MicroTalk decoder's allocation and bit reader; see docs/Sound.md.
#include "SndMicroTalk.h"
namespace Snd {
static unsigned int bitmask[9] = {0, 1, 3, 7, 0xf, 0x1f, 0x3f, 0x7f, 0xff};

void *CMTBLKDecf::operator new(unsigned int size) {
    return CODANew(size);
}

void CMTBLKDecf::operator delete(void *p) {
    CODADelete(p);
}

static unsigned int getbits(UTALKSTATE_CODA *s, int n) {
    unsigned int v = s->bits & bitmask[n];
    s->bits >>= n;
    s->count -= n;
    if (s->count < 8) {
        s->bits |= *s->ptr << s->count;
        s->ptr++;
        s->count += 8;
    }
    return v;
}

void discardbits(UTALKSTATE_CODA *s, int n) {
    s->bits >>= n;
    s->count -= n;
    if (s->count < 8) {
        s->bits |= *s->ptr << s->count;
        s->ptr++;
        s->count += 8;
    }
}

// The two counters are shared by both arms, as the register use shows: i holds the output
// index in the Huffman arm and the two-bit code in the other, j the opposite.
}
