// The memory card popups: screens run from inside the save and load flow. Scoped view; see docs/Frontend.md.
#ifndef GAME_SAVE_GAME_POPUPS_H
#define GAME_SAVE_GAME_POPUPS_H
#pragma interface
#include "FEngElements.h"
#include "GameSettings.h"
#include "PackageHandlers.h"
#include "Shell.h"
#include "SoundEvents.h"
#include "StringCRC.h"
#include <stdio.h>
#include <string.h>
inline void *operator new(unsigned int size, const char *label) { return DWI_alloc(label, size, 1024); }
class CMemoryCardPopup : public CSaveGamePopup {
  public:
    static void InitClass();
    static void ShutdownClass();
    static CMemoryCardPopup *Instance();
    CMemoryCardPopup();
    ~CMemoryCardPopup();
    void OnUIEvent(unsigned int, FEObject *);
    void OnSoundEvent(unsigned int, FEObject *);
    void OnPush();
    void HideSelections();
    void ShowSelections();
    void Display(const wchar_t unsigned *, const wchar_t unsigned *, const wchar_t unsigned *, const wchar_t unsigned *, const wchar_t unsigned *);
    void RequestReply();
    int GetReply();
    static CMemoryCardPopup *s_pInstance;
    int waitingForReply;  // +0x20
    int replied;          // +0x24
    int reply;            // +0x28
    FEString *message;    // +0x2c
    FEString *labels[4];  // +0x30; "Label%d".
    int selectable[4];    // +0x40
};
class CSaveGameKeyboardPopup : public CSaveGamePopup {
  public:
    static CSaveGameKeyboardPopup *Instance();
    static void Shutdown();
    CSaveGameKeyboardPopup();
    ~CSaveGameKeyboardPopup();
    void CreateName();
    void OnUIEvent(unsigned int, FEObject *);
    void OnSoundEvent(unsigned int, FEObject *);
    void AddToNameString(char *, char);
    void BackspaceNameString(char *);
    void LocalizeString(const char *, const char *);
    static CSaveGameKeyboardPopup *m_sInstance;
    unsigned char unknown_20[0x2c - 0x20];
    int nameStarted;      // +0x2c; the first letter replaces the default name.
};
class CSaveGameLanguageScreen : public CSaveGamePopup {
  public:
    static CSaveGameLanguageScreen *Instance();
    CSaveGameLanguageScreen();
    ~CSaveGameLanguageScreen();
    void SelectLanguage();
    void OnUIEvent(unsigned int, FEObject *);
    void OnSoundEvent(unsigned int, FEObject *);
    int GetLocaleID(int);
    int GetButtonName(int);
    unsigned long buttonHashes[5]; // +0x20; "Language Button %d".
    int languages[5];              // +0x34; language codes, one per button.
    unsigned char unknown_48[4];
    int idleFrames;                // +0x4c; the movie plays after 900 frames here.
};
#endif
