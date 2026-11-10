// AI-assisted reconstruction of the sound event base's pause flag and end; see docs/Sound.md.
#include "SoundManager.h"
// Method-only view of the class; its table and data members are not reconstructed here.
class Moh3SoundEvent {
  public:
    static int s_paused;
    static void Pause(bool);
    int EndEvent(unsigned int);
};

void Moh3SoundEvent::Pause(bool paused) {
    s_paused = paused;
    g_npcVocalMan.paused = paused;
}

int Moh3SoundEvent::EndEvent(unsigned int id) {
    g_SoundMan.GetSchedule().RemoveSound(id);
    return 0;
}
