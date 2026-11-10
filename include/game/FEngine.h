// FEngine: FEng's root object. Scoped view; see docs/Frontend.md.
#ifndef GAME_FENGINE_H
#define GAME_FENGINE_H
#pragma interface
#include "FEngLists.h"

class FEGameInterface;
class FEMessageResponse;
class FEJoyPad;
class FEMessageNode : public FEMinNode {
  public:
    FEObject *from;        // +0x0c
    FEObject *to;          // +0x10
    FEPackage *package;    // +0x14
    unsigned long message; // +0x18
    unsigned long mask;    // +0x1c
    FEMessageNode() {}
    virtual ~FEMessageNode();
};

// A queued package operation; the type values are observed in the Queue methods.
class FEPackageCommand : public FENode {
  public:
    int type;              // +0x14
    unsigned long mask;    // +0x18
    FEPackage *package;    // +0x1c
    FEPackageCommand() {}
    virtual ~FEPackageCommand();
};

class FEngine {
  public:
    static unsigned long SysGUID;
    void Render();
    unsigned long GetCombinedJoyPadActiveMask() const;
    void SetNumJoyPads(unsigned char);
    void ResetJoyPad();
    void SetExecution(bool);
    void AddToIdleList(FEPackage *);
    FEPackage *FindIdlePackage(const char *) const;
    FEPackage *GetFirstLibrary() const;
    void AddToLibraryList(FEPackage *);
    void RemoveFromLibraryList(FEPackage *);
    FEPackage *FindLibraryPackage(unsigned long) const;
    FEObject *FindObject(unsigned long, const char *) const;
    FEObject *FindObject(const char *, const char *) const;
    void StartCheckMovieDemoScreen();
    void QueueMessage(unsigned long, FEObject *, FEPackage *, FEObject *, unsigned long);
    void QueuePackagePush(const char *, FEPackage *, unsigned long);
    void QueuePackageSwitch(const char *, FEPackage *);
    void QueuePackagePop(FEPackage *);
    void QueuePackageUserTransfer(FEPackage *, bool, unsigned long);
    void UnloadLibraryPackage(FEPackage *);
    void CheckMovieDemoScreen(char *);
    void SendMessageToGame(unsigned long, FEObject *, FEPackage *, unsigned long);
    int ProcessListBoxResponses(FEObject *, FEPackage *, unsigned long);
    int ProcessCodeListBoxResponses(FEObject *, FEPackage *, unsigned long);
    void ProcessObjectMessage(FEObject *, FEPackage *, unsigned long, unsigned long);
    void ProcessGlobalMessage(FEPackage *, unsigned long, unsigned long);
    void ProcessResponses(FEMessageResponse *, FEObject *, FEPackage *, unsigned long);
    void RecordLastPackageButton(unsigned long, unsigned long);
    unsigned long RecallLastPackageButton(unsigned long);
    int RecordPackageMarker(const char *);
    char *RecallPackageMarker();
    void ClearPackageMarkers();

    int checkMovieDemoScreen;    // +0x0000
    int executing;               // +0x0004
    int resetRequested;          // +0x0008; set by ShellManager::Reset. Name inferred.
    unsigned char unknown_c[0x18 - 0xc];
    FEJoyPad *joyPads;           // +0x0018
    unsigned char unknown_1c[0x4c - 0x1c];
    unsigned char joyPadState[76]; // +0x004c; cleared as a block by ResetJoyPad.
    unsigned char unknown_98[0xe8 - 0x98];
    int joyPadCount;             // +0x00e8
    unsigned short activeMask;   // +0x00ec
    unsigned char unknown_ee[0xf0 - 0xee];
    FEList packages;             // +0x00f0
    FEList idlePackages;         // +0x0100
    FEList libraries;            // +0x0110
    FEGameInterface *game;       // +0x0120
    unsigned char unknown_124[0x4128 - 0x124];
    FEMinList messageQueue;      // +0x4128
    FEMinList commandQueue;      // +0x4138
    unsigned char unknown_4148[0x415c - 0x4148];
    int markerCount;             // +0x415c
    char markers[16][256];       // +0x4160
    int lastButtonIndex;         // +0x5160
    struct { unsigned long package; unsigned long button; } lastButtons[32]; // +0x5164
    unsigned char unknown_5264[4];
    int debugMessages;           // +0x5268; passes queued messages to the game interface.
    signed char lastAcceptPad;   // +0x526c; read by ShellManager::GetIndexPadLastPressedAccept.
};
#endif
