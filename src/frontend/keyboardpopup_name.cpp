// CSaveGameKeyboardPopup's sounds and name editing; see docs/Frontend.md.
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
void CSaveGameKeyboardPopup::OnSoundEvent(unsigned int event, FEObject *) {
    SoundAliasEvent sound;
    if (event == FEHashUpper("Button_Highlight")) {
        SetAlias(sound, "MenuSelect");
        sound.SendEvent();
    } else if (event == FEHashUpper("Button_Pressed")) {
        SetAlias(sound, "Pause");
        sound.SendEvent();
    }
}
void CSaveGameKeyboardPopup::AddToNameString(char *name, char letter) {
    if (!nameStarted) {
        name[0] = 0;
        nameStarted = 1;
    }
    int length = strlen(name);
    if (length < 8) {
        name[length] = letter;
        name[length + 1] = 0;
    }
}
void CSaveGameKeyboardPopup::BackspaceNameString(char *name) {
    int length = strlen(name);
    if (length > 0) {
        name[length - 1] = 0;
    }
}
void CSaveGameKeyboardPopup::LocalizeString(const char *object, const char *label) {
    FEString *string = (FEString *)package->FindObjectByName(object);
    if (string) {
        FEWideString *text = g_Shell.localizer->GetString(label, 0);
        string->text = text->text;
        string->changed = 1;
    }
}
