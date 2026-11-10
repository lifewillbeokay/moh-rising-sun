#ifndef GAME_SOUND_BUSS_H
#define GAME_SOUND_BUSS_H
// AI-assisted runtime view of the sound buss; see docs/Sound.md. Class and method names come from the
// symbol table; member names are inferred from use. The 216-byte record size comes from the free list.
class SoundEntry;
struct BussEvent {
    int time;          // +0: compared with the clock.
    short timeValue;   // +4: copied into time on insertion.
    unsigned char unknown_6[6];
    float target;      // +12: becomes the volume when the buss stops on this event.
    unsigned char unknown_10[8];
};
class cSoundBuss {
  public:
    cSoundBuss *next;       // +0
    BussEvent events[8];    // +4
    short active;           // +196
    unsigned int crc;       // +200
    short currentEvent;     // +204
    short eventCount;       // +206
    float clock;            // +208
    float volume;           // +212
    cSoundBuss();
    bool ActivateBuss();
    void IncrementClock(float);
    void DeactivateBuss();
    void RetroFit(SoundEntry *);
    void UpdateBussEntry(SoundEntry *);
};
void ShutdownSoundBussMan();
cSoundBuss *GetFreeSoundBuss();
typedef char cSoundBussSizeCheck[sizeof(cSoundBuss) == 216 ? 1 : -1];
#endif
