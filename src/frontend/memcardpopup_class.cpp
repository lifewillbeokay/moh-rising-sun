// CMemoryCardPopup's instance and constructor; see docs/Frontend.md.
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
