// AI-assisted scoped reconstruction from GR8E69; see docs/Sound.md.
#ifndef GAME_SOUND_AMBIENCE_H
#define GAME_SOUND_AMBIENCE_H
#include "CVector3.h"

// Member-only view of the 76-byte ambient-stream event: only the fields the
// ambient-track functions touch are named. The table pointer and base classes
// are not declared.
class AmbientStreamSoundEvent {
  public:
    unsigned char unknown_00[0x24];
    int field_24;
    float field_28;
    unsigned char unknown_2c[0x34 - 0x2c];
    float volume;            // AmbientTrack_Volume
    unsigned char unknown_38[0x3c - 0x38];
    short field_3c, field_3e;
    unsigned char unknown_40[0x44 - 0x40];
    int selectedTrack;       // AmbientTrack_Select
    int currentTrack;        // SoundGetAmbientTrackEvent
    int SendEvent();
    int UpdateEvent(unsigned int, int);
};

void SoundGetVolumes(float &, float &);
void SoundGetOneShotBankID(int &);
void SoundGetAmbientTrackEvent(int &);
void SoundGetMusicEventID(int &);
void AmbientTrack_Select(int);
void AmbientTrack_Volume(float);
void AmbientTrack_SetLocation(const CVector3 *);

#endif
