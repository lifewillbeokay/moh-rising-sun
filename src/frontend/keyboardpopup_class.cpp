// CSaveGameKeyboardPopup's instance and constructor; see docs/Frontend.md.
#include "SaveGamePopups.h"
CSaveGameKeyboardPopup *CSaveGameKeyboardPopup::Instance() {
    if (!m_sInstance) {
        m_sInstance = new ("source/SaveGame/SaveGameKeyboardPopup.cpp:47") CSaveGameKeyboardPopup;
    }
    return m_sInstance;
}
void CSaveGameKeyboardPopup::Shutdown() {
    if (m_sInstance) {
        delete m_sInstance;
        m_sInstance = 0;
    }
}
CSaveGameKeyboardPopup::CSaveGameKeyboardPopup() : CSaveGamePopup("ProfileNameEntry.fng") { nameStarted = 0; }
CSaveGameKeyboardPopup::~CSaveGameKeyboardPopup() {}
