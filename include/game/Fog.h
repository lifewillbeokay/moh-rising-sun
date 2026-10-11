#ifndef GAME_FOG_H
#define GAME_FOG_H

// AI-assisted scoped storage view from GR8E69; see docs/Script.md.
// Member names are descriptive. This prefix is not a complete class declaration
// and must not be used to allocate or construct the original 96-byte object.
struct CFog {
    unsigned char unknown00[0x44];
    unsigned int colour; // Packed red, green, blue, alpha bytes, high to low.
    unsigned char unknown48[8];
    float start;
    unsigned char unknown54[4];
    float end;
};

extern CFog g_fog;
typedef char CFogPrefixSizeCheck[sizeof(CFog) == 0x5c ? 1 : -1];

#endif
