// Scoped 24-byte options layout already used by SNDplaysetdef; see docs/Sound.md.
#ifndef GAME_SOUND_PLAY_OPTIONS_H
#define GAME_SOUND_PLAY_OPTIONS_H
struct SNDPLAYOPTS {
    unsigned char byte0;
    unsigned char byte1;
    unsigned char byte2;
    unsigned char byte3;
    unsigned char byte4;
    unsigned char byte5;
    short short6;
    short short8;
    short shortA;
    short shortC;
    short shortE;
    short short10;
    short short12;
    unsigned short short14;
    short short16;
};
extern "C" int SNDplaysetdef(SNDPLAYOPTS*);
typedef char SoundPlayOptionsSizeCheck[sizeof(SNDPLAYOPTS) == 24 ? 1 : -1];
#endif
