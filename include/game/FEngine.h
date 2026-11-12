// FEngine: FEng's root object. Scoped view; see docs/Frontend.md.
#ifndef GAME_FENGINE_H
#define GAME_FENGINE_H
#include "FEMouse.h"
#pragma interface
#include "FEngLists.h"
#include "FEngMath.h"

class FEGameInterface;
class FEMessageResponse;
class FETypeNode;
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

// Object types by hash, built by Startup.
class FETypeLib {
  public:
    FEList types;    // +0x0
    long unknown_10; // +0x10
    FETypeLib() : unknown_10(0) {}
    int Startup();
    FETypeNode *CreateBaseObjectType(const char *);
    FETypeNode *CreateImageObjectType(const char *);
    FETypeNode *FindType(unsigned long);
};

class FEngine {
  public:
    static unsigned long SysGUID;
    void Render();
    unsigned long GetCombinedJoyPadActiveMask() const;
    void SetNumJoyPads(unsigned char);
    void ResetJoyPad();
    void SetExecution(bool);
    FEngine();
    void RenderGroup(FEGroup *, FEMatrix4 &, FEMatrix4 &, unsigned short);
    void RenderObject(FEObject *, FEMatrix4 &, unsigned short);
    ~FEngine();
    void Update(long);
    void ProcessPadsForPackage(FEPackage *);
    void ProcessMouseForPackage(FEPackage *);
    void ProcessPackageCommands();
    void ReleaseAll();
    FEPackage *PushPackage(const char *, unsigned char, unsigned long);
    void AddToIdleList(FEPackage *);
    FEPackage *FindIdlePackage(const char *) const;
    FEPackage *GetFirstLibrary() const;
    FEPackage *LoadPackage(const void *, bool);
    int UnloadPackage(FEPackage *);
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
    void ProcessMessageQueue();
    void ProcessResponses(FEMessageResponse *, FEObject *, FEPackage *, unsigned long);
    void RecordLastPackageButton(unsigned long, unsigned long);
    unsigned long RecallLastPackageButton(unsigned long);
    int RecordPackageMarker(const char *);
    char *RecallPackageMarker();
    void ClearPackageMarkers();

    int checkMovieDemoScreen;    // +0x0000
    int executing;               // +0x0004
    int resetRequested;          // +0x0008; set by ShellManager::Reset. Name inferred.
    int unknown_c;               // +0x000c
    bool unknown_10;             // +0x0010; passed to FEPackageReader::Load as its first flag.
    bool unknown_14;             // +0x0014; and its second.
    FEJoyPad *joyPads;           // +0x0018
    FEMouse mouse;               // +0x001c
    int unknown_40;              // +0x0040
    int unknown_44;              // +0x0044
    union { unsigned char unknown_48[4]; unsigned long consumedDirections; }; // +0x48; inferred input-repeat mask.
    union { unsigned char joyPadState[76]; unsigned long repeatAmounts[19]; }; // +0x4c
    union { unsigned char unknown_98[0xe4 - 0x98]; FEObject *pressedObjects[19]; }; // +0x98
    int unknown_e4;              // +0x00e4
    int joyPadCount;             // +0x00e8
    unsigned short activeMask;   // +0x00ec
    unsigned short renderContextCount; // +0x00ee
    FEList packages;             // +0x00f0
    FEList idlePackages;         // +0x0100
    FEList libraries;            // +0x0110
    FEGameInterface *game;       // +0x0120
    FEObjectSorter<1024> sorter; // +0x0124
    FEMinList messageQueue;      // +0x4128
    FEList commandQueue;         // +0x4138
    FETypeLib typeLib;           // +0x4148
    int markerCount;             // +0x415c
    char markers[16][256];       // +0x4160
    int lastButtonIndex;         // +0x5160
    struct { unsigned long package; unsigned long button; } lastButtons[32]; // +0x5164
    int unknown_5264;            // +0x5264
    int debugMessages;           // +0x5268; passes queued messages to the game interface.
    signed char lastAcceptPad;   // +0x526c; read by ShellManager::GetIndexPadLastPressedAccept.
};
#endif
