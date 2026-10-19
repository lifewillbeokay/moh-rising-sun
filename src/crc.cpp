// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "CRC.h"

void CCRC::Reset() {
    value = initial;
}

void CCRC::operator+=(unsigned char byte) {
    unsigned int top = 1U << (width - 1);
    unsigned int input = byte;
    if (reflect)
        input = u4Reflect(input, 8);
    value ^= input << (width - 8);
    for (int i = 0; i < 8; ++i) {
        if (value & top)
            value = (value << 1) ^ polynomial;
        else
            value <<= 1;
        value &= mask;
    }
}

unsigned int CCRC::u4Reflect(unsigned int input, unsigned int bits) {
    unsigned int remaining = input;
    for (unsigned int i = 0; i < bits; ++i) {
        if (remaining & 1)
            input |= 1U << (bits - 1 - i);
        else
            input &= ~(1U << (bits - 1 - i));
        remaining >>= 1;
    }
    return input;
}
