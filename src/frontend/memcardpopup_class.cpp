// CMemoryCardPopup lifecycle and UI/sound dispatch; see docs/research/save-popups.md.
#include "SaveGamePopups.h"
void CMemoryCardPopup::InitClass() {
    s_pInstance = new ("source/SaveGame/MemoryCardPopup.cpp:31") CMemoryCardPopup;
}
void CMemoryCardPopup::ShutdownClass() {
    if (s_pInstance) {
        delete s_pInstance;
    }
    s_pInstance = 0;
}
CMemoryCardPopup *CMemoryCardPopup::Instance() { return s_pInstance; }
CMemoryCardPopup::CMemoryCardPopup() : CSaveGamePopup("MemoryCardPopup.fng") {
    waitingForReply = 0;
    replied = 0;
    reply = 0;
    FEString *object = (FEString *)engine->FindObject("Message", 0);
    message = object;
    object->text = "";
    object->changed = 1;
    for (unsigned int i = 0; i < 4; i++) {
        char name[16];
        sprintf(name, "Label%d", i);
        FEString *label = (FEString *)engine->FindObject(name, 0);
        labels[i] = label;
        if (label) {
            label->text = "";
            label->changed = 1;
        }
        selectable[i] = 0;
    }
    HideSelections();
}
CMemoryCardPopup::~CMemoryCardPopup() {}

void CMemoryCardPopup::OnUIEvent(unsigned int event, FEObject *) {
    static const unsigned long uOptionEvents[4] = { FEHashUpper("Pad_Button0"), FEHashUpper("Pad_Button1"), FEHashUpper("Pad_Button2"), FEHashUpper("Pad_Button3") };
    for (unsigned int i = 0; i < 4; ++i)
        if (event == uOptionEvents[i]) { replied = 1; reply = i; }
}

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

void CMemoryCardPopup::OnPush() {
    FEString *string;
    string = message;
    if (string) { string->text = ""; string->flags |= 0x400000; }
    string = labels[0];
    if (string) { string->text = ""; string->flags |= 0x400000; }
    string = labels[1];
    if (string) { string->text = ""; string->flags |= 0x400000; }
    string = labels[2];
    if (string) { string->text = ""; string->flags |= 0x400000; }
    string = labels[3];
    if (string) { string->text = ""; string->flags |= 0x400000; }
    waitingForReply = 0; replied = 0; reply = 0; displayed = 0;
}
