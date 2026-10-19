// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#ifndef GAME_CRC_H
#define GAME_CRC_H

// Seven-word storage established by the 28-byte animated-texture allocation,
// initialization and three CCRC methods. Member names are descriptive; the
// original constructors, access control and return-type spelling are unknown.
class CCRC {
  public:
    unsigned int width;      // +0x00: number of CRC bits.
    unsigned int mask;       // +0x04: retained bits after each step.
    unsigned int polynomial; // +0x08
    unsigned int initial;    // +0x0c
    unsigned int reflect;    // +0x10: nonzero selects input/output reflection.
    unsigned int finalXor;   // +0x14
    unsigned int value;      // +0x18: running CRC.

    void Reset();
    void operator+=(unsigned char);
    unsigned int u4Reflect(unsigned int, unsigned int);
};

#endif
