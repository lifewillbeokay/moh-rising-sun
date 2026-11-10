// CMemoryCardPopup's sound events; see docs/Frontend.md.
#include "SaveGamePopups.h"
inline SoundEvent::SoundEvent() : emitter(0), listener(0), field_08(0), field_1c(0) {
    int i = 3;
    int zero = 0;
    int *p = field_10;
    for (; i != 0; --i)
        *p++ = zero;
    field_0c = 0.0f;
}
inline SoundAliasEvent::SoundAliasEvent() : SoundEvent(), alias(0) {}
inline SoundAliasEvent::~SoundAliasEvent() {}
static inline void SetAlias(SoundAliasEvent &event, const char *name) { event.alias = GetStringCRC(name); }
void CMemoryCardPopup::OnSoundEvent(unsigned int event, FEObject *) {
    SoundAliasEvent sound;
    if (event == FEHashUpper("Button_Highlight")) {
        if (!g_GameFlow.levelIsShell) {
            SetAlias(sound, "MenuSelect");
        } else {
            SetAlias(sound, "DrumMvmt");
        }
        sound.SendEvent();
    } else if (event == FEHashUpper("Button_Pressed")) {
        if (!g_GameFlow.levelIsShell) {
            SetAlias(sound, "Pause");
        } else {
            SetAlias(sound, "DrumSelect");
        }
        sound.SendEvent();
    }
}
