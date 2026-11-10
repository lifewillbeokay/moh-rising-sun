// A fragment of the original BitBuffer.cpp: a 2048-byte buffer read and written a bit at a time. Names
// come from the symbol table. The current reader and writer and their positions are the file's private
// statics, reached here as externals.
#include <string.h>

class BitBuffer {
  public:
    void SaveBegin();
    void SaveEnd();
    void LoadBegin();
    void LoadEnd();
    unsigned int Load(unsigned int bits);
    int Save(unsigned int value, unsigned int bits);
    int Used() const;
    char data[2048];
};

extern BitBuffer *g_pCurrentReader;
extern BitBuffer *g_pCurrentWriter;
extern unsigned int g_uRByte;
extern unsigned int g_uWByte;
extern unsigned char g_uRBitsLeft;
extern unsigned char g_uWBitsLeft;

void BitBuffer::SaveBegin() {
    g_pCurrentWriter = this;
    memset(this, 0, 2048);
    g_uWByte = 0;
    g_uWBitsLeft = 8;
}

void BitBuffer::SaveEnd() { g_pCurrentWriter = 0; }

void BitBuffer::LoadBegin() {
    g_pCurrentReader = this;
    g_uRByte = 0;
    g_uRBitsLeft = 8;
}

void BitBuffer::LoadEnd() { g_pCurrentReader = 0; }

unsigned int BitBuffer::Load(unsigned int bits) {
    unsigned int available = 0;
    if (g_uRByte <= 2047)
        available = (2047 - g_uRByte) * 8 + g_uRBitsLeft;
    if (bits > available)
        return 0;
    unsigned int value = 0;
    for (unsigned int i = 0; i < bits; i++) {
        if ((data[g_uRByte] >> (8 - g_uRBitsLeft)) & 1)
            value |= 1 << i;
        if (--g_uRBitsLeft == 0) {
            g_uRBitsLeft = 8;
            g_uRByte++;
        }
    }
    if (bits >= 2 && bits <= 31) {
        unsigned int signBit = 1 << (bits - 1);
        if (value & signBit)
            value |= -1 << bits;
    }
    return value;
}

int BitBuffer::Save(unsigned int value, unsigned int bits) {
    unsigned int available = 0;
    if (g_uWByte <= 2047)
        available = (2047 - g_uWByte) * 8 + g_uWBitsLeft;
    if (bits > available)
        return 0;
    unsigned int remaining = bits;
    while (remaining) {
        if (value & 1)
            data[g_uWByte] |= 1 << (8 - g_uWBitsLeft);
        value >>= 1;
        remaining--;
        if (--g_uWBitsLeft == 0) {
            g_uWBitsLeft = 8;
            g_uWByte++;
            data[g_uWByte] = 0;
        }
    }
    return 1;
}

int BitBuffer::Used() const {
    int used = g_uWByte;
    if (g_uWBitsLeft <= 7)
        used++;
    return used;
}
