// AI-assisted reconstruction from GR8E69; see docs/Sound.md.
#include "SoundManager.h"
extern "C" int SNDtimeremaining(int);
extern "C" void SNDSYS_entercritical();
extern "C" void SNDSYS_leavecritical();
extern "C" int SNDstop(int);
int SoundEntryBank::IsFinished() {
    if (SNDtimeremaining(handle) < 0) {
        Stop();
        handle = -12;
        return 1;
    }
    return 0;
}
void SoundEntryBank::SetFade(float, float, bool) {}
void SoundEntryBank::ResolveSlotNum() {
    if (slotMode > 1) patch = g_SoundMan.slotMap[patch + field_dc];
}
int SoundEntryBank::Stop() {
    SNDSYS_entercritical();
    int result = SNDstop(handle);
    SNDSYS_leavecritical();
    return result < 0 ? -1 : 0;
}
