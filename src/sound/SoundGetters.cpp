// AI-assisted reconstruction from GR8E69; see docs/Sound.md.
#include "SoundAmbience.h"
// Original sound.cpp storage remains external context.
extern int g_bDisableSound;
extern AmbientStreamSoundEvent gAmbientTrackEvent;
extern int g_OneShotBankID;
extern int gMusicCurEvent;
extern float g_sfxVolume;
extern float g_musicVolume;

void SoundGetVolumes(float &sfx, float &music) {
    if (g_bDisableSound)
        return;
    sfx = g_sfxVolume;
    music = g_musicVolume;
}
void SoundGetOneShotBankID(int &id) { id = g_OneShotBankID; }
void SoundGetAmbientTrackEvent(int &track) { track = gAmbientTrackEvent.currentTrack; }
void SoundGetMusicEventID(int &id) { id = gMusicCurEvent; }
