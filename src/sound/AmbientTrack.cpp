// AI-assisted reconstruction from GR8E69; see docs/Sound.md.
#include "SoundAmbience.h"
// Original sound.cpp storage remains external context.
void DebugMsg(const char *, ...);
void err(char *, ...);
extern int g_bDisableSound;
extern int g_hAmbientTrack;
extern AmbientStreamSoundEvent gAmbientTrackEvent;

static void AmbientTrack_Start() {
    if (g_bDisableSound)
        return;
    gAmbientTrackEvent.field_24 = 0;
    gAmbientTrackEvent.field_28 = 0.0f;
    gAmbientTrackEvent.field_3c = 70;
    gAmbientTrackEvent.field_3e = 70;
    g_hAmbientTrack = gAmbientTrackEvent.SendEvent();
    if (g_hAmbientTrack < 0) {
        err("AmbientTrack_Start():  SendEvent() FAILED!!! (error == %d)\n", g_hAmbientTrack);
        DebugMsg("\n!!! WARNING !!!\nAmbientTrack_Start():  SendEvent() FAILED!!! (error == %d)\n", g_hAmbientTrack);
        g_hAmbientTrack = -12;
    }
}
void AmbientTrack_Select(int track) {
    if (g_bDisableSound)
        return;
    gAmbientTrackEvent.selectedTrack = track;
    if (g_hAmbientTrack == -12)
        AmbientTrack_Start();
    else if (track != gAmbientTrackEvent.currentTrack)
        g_hAmbientTrack = gAmbientTrackEvent.UpdateEvent(g_hAmbientTrack, 1);
}
void AmbientTrack_Volume(float volume) {
    if (g_bDisableSound)
        return;
    if (g_hAmbientTrack == -12)
        return;
    gAmbientTrackEvent.volume = volume;
}
