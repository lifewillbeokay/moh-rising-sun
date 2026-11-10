// FEng objects and package handlers; scoped views, see docs/Frontend.md.
#ifndef GAME_FENG_OBJECTS_H
#define GAME_FENG_OBJECTS_H
#include "FEngLists.h"
#include "FEngStrings.h"
#include "ShellManager.h"
#include "FEScript.h"
// The object fields the string objects add.
class FEStringObject : public FEObject {
  public:
    unsigned char unknown_58[0x60 - 0x58];
    FEWideString text;            // +0x60
};

class PackageHandler {
  public:
    virtual ~PackageHandler();
    virtual int Init(const char *);
    virtual void HandleTick();
    virtual int HandleEvent(unsigned int, FEObject *, unsigned long) = 0;
    virtual int HandleSoundEvent(unsigned int, FEObject *);
    int SetScript(FEPackage *, const char *, const char *);
    int SetScript(FEPackage *, unsigned long, const char *);
    int GetCurrentLanguage();
    int SetObjectListIndex(char *, unsigned int);
    int SetNextObjectListIndex(char *);
    int SetPrevObjectListIndex(char *);
    int GetObjectListIndex(char *);
    void SetProfileNameOnScreen(int);
    void Set2ProfileNamesOnScreen();
    int SetGroupString(FEGroup *, const char *, char *);
    int SetGroupString(unsigned int, unsigned int, FEWideString *);
    int SetGroupString(unsigned int, unsigned int, const char *);
    int SetGroupShowOnly(FEGroup *, unsigned long);
    int SetGroupShowOnly(FEGroup *, const char *);
    int SetGroupShowOnly(unsigned long, unsigned long);
    void QueueMessage(unsigned int);
    int HandleMovieEvent(unsigned int, FEObject *);
    int HandleCoopLevelStartEvent(unsigned int, FEObject *);
    int HandleFEngQueries(unsigned int, FEObject *);
    int HandleSaveGameEvent(unsigned int, FEObject *);
    class ShellManager *shell; // +0x00
    class FEngine *engine;     // +0x04
    FEPackage *package;        // +0x08
    char name[0x40];           // +0x0c
    void SetObjectString(FEPackage *, const char *, const char *);
    void SetObjectString(FEPackage *, const char *, FEWideString *);
    void SetObjectString(FEPackage *, const char *, const wchar_t unsigned *);
    void EnableButton(FEObject *);
    void DisableButton(FEObject *);
    void EnableButton(FEPackage *, const char *);
    void DisableButton(FEPackage *, const char *);
    void EnableButton(FEPackage *, unsigned long);
    void DisableButton(FEPackage *, unsigned long);
};
#endif
