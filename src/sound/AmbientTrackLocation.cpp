// AI-assisted reconstruction from GR8E69; see docs/Sound.md.
#include "SoundAmbience.h"
// Original sound.cpp storage remains external context.
extern int g_bDisableSound;
extern int g_hAmbientTrack;
extern int g_ambientSourceOnPlayer;
extern CVector3 g_ambientSourcePos;

void AmbientTrack_SetLocation(const CVector3 *location) {
    if (g_bDisableSound)
        return;
    if (g_hAmbientTrack == -12)
        return;
    g_ambientSourceOnPlayer = location == 0;
    if (g_ambientSourceOnPlayer)
        return;
    g_ambientSourcePos = *location;
}
