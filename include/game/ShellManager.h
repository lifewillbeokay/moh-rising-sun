// ShellManager: the front end's connection to FEng. Scoped view; see docs/Frontend.md.
// Only the members and virtual slots reconstructed code touches are named.
#ifndef GAME_SHELL_MANAGER_H
#define GAME_SHELL_MANAGER_H
#pragma interface
#include "ConfigFile.h"
#include "FEngLists.h"
#include "FEngine.h"

class FEMatrix4 { public: void Identify(); };
class FEMouseInfo;
struct FEObjectListEntry { void *object; int unknown_4; };
class ShellLoader {
  public:
    int LoadPackage(const char *, unsigned char **, bool &);
    int LoadResources(FEPackage *, long, FEResourceRequest *);
    int UnloadResources(FEPackage *, long, FEResourceRequest *);
};
enum EMovieType { EMovieTypeUnknown0 = 0 };
struct _ShellMoviePaths {
    char movie[64];
    char text[64];
    char time[64];
};
// Paths from game.cfg, named by its keys; the first index is the movie type.
class CShellMovieSettings {
  public:
    int loaded;                    // +0x0000; tested by InitMovies.
    char fontName[64];             // +0x0004
    char backName[64];             // +0x0044
    _ShellMoviePaths paths[4][11]; // +0x0084; historical, veteran, anim, letter.
    _ShellMoviePaths demo;         // +0x2184
    _ShellMoviePaths credits;      // +0x2244
    CShellMovieSettings();
    ~CShellMovieSettings();
    void Reset();
    void Load(CConfigFile &);
    void LoadSection(CConfigFile &, int);
    _ShellMoviePaths *GetMoviePathsStruct(EMovieType, int);
};
// One joined co-op player. coopJoin[0].port is what IsCoopOnTop compares.
struct SCoopJoinInfo {
    FEPackage *package;  // +0x0
    signed char port;    // +0x4
    unsigned long mask;  // +0x8
};
class FEGameInterface {
  public:
    virtual void Slot0();
    virtual void Slot1();
    virtual int LoadResources(FEPackage *, long, FEResourceRequest *);
    virtual int UnloadResources(FEPackage *, long, FEResourceRequest *);
    virtual void Slot4();
    virtual int PackageWillUnload(FEPackage *);
    virtual int CanUnloadPackages(); // Name inferred from its one use.
    virtual void NotificationMessage(unsigned long, FEObject *, unsigned long, unsigned long);
    virtual void Slot8();
    virtual void Slot9();
    virtual void Slot10();
    virtual void Slot11();
    virtual void Slot12();
    virtual void Slot13();
    virtual void RenderObject(void *); // +0x78 in the table.
    virtual void Slot15();
    virtual void Slot16();
    virtual void Slot17();
    virtual void Slot18();
    virtual void Slot19();
    virtual void Slot20();
    virtual void Slot21();
    virtual void DebugMessageQueued(unsigned long, FEObject *, FEPackage *, FEObject *, unsigned long);
};
class ShellManager : public FEGameInterface {
  public:
    int Startup(char *);
    void Draw();
    void DrawUnplugged();
    int GetPackageData(const char *, unsigned char **, bool &);
    int LoadResources(FEPackage *, long, FEResourceRequest *);
    int UnloadResources(FEPackage *, long, FEResourceRequest *);
    void BeginPackageRendering(FEPackage *);
    void EndPackageRendering(FEPackage *);
    void RenderObjectList(FEObjectListEntry *, unsigned long);
    void GetViewTransformation(FEMatrix4 *);
    void MoveShellCamera();
    unsigned long GetCombinedJoyPadActiveMask() const;
    void GetMouseInfo(FEMouseInfo &);
    void SetMovieName(int, char *);
    void PlayMovie(_ShellMoviePaths *, bool);
    void PlayMovie(EMovieType, int, bool);
    FENode *GetPauseScreen();
    void SetNumberJoyPads(unsigned char);
    int LocalizePackage(FEPackage *);
    void LocalizeObjects(FEObject *);
    void DebugMessageQueued(unsigned long, FEObject *, FEPackage *, FEObject *, unsigned long);
    void DebugMessageProcessed(unsigned long, FEObject *, FEObject *, FEPackage *, unsigned long);
    int HasScreen(const char *);
    void InitMovies();
    void Push(const char *);
    void Swap(const char *, FEPackage *);
    void DoMPControllerChecks();
    void ResetActiveControllerIndicies();
    void SetActiveControllerIndicies();
    void RemoveMPViewPorts();
    void UpdateUnplugged();
    void ResetCoopJoinInfo(signed char);
    void SetCoopJoinInfo(FEPackage *, unsigned long);
    void DoCoopJoinInfoUpdating();
    void SetStartMPDataFromShell();
    void Reset(const char *);
    void Process(unsigned int);
    void ResetCoopJoinInfoFromPortNum(unsigned char);
    void DoCoopControllerChecks();
    int IsCoopOnTop(unsigned char);
    int IsCoopOnTopFromPortNum(unsigned char);
    signed char GetDeviceIndexFromControlMask(unsigned long);
    signed char GetPortNumFromControlMask(unsigned long);
    void SetKeyboardString(const char *);
    unsigned char GetNumCoopPlayers();
    signed char GetIndexPadLastPressedAccept();

    char keyboardString[0x20];        // +0x04; the extent past the 17 bytes copied is unknown.
    int selectedLevel;                // +0x24; CGameSettings reads per-level stats at this - 1.
    unsigned char unknown_28[0x3c - 0x28];
    ShellLoader *loader;              // +0x3c
    FEngine *engine;                  // +0x40
    unsigned char unknown_44[0x18a0 - 0x44];
    unsigned char mpPlayerCount;      // +0x18a0; copied to the settings when the match starts. Name inferred.
    unsigned char coopJoinBits;       // +0x18a1; bit n per player.
    unsigned char unknown_18a2[0x18a8 - 0x18a2];
    int coopPausePlayer;              // +0x18a8; set when a co-op game opens the save screen. Name inferred.
    unsigned char unknown_18ac[0x18b8 - 0x18ac];
    int inCoop;                       // +0x18b8; UpdateUnplugged picks the co-op controller checks.
    int inMultiplayer;                // +0x18bc; UpdateUnplugged picks the multiplayer checks.
    unsigned char unknown_18c0[0x18c4 - 0x18c0];
    CShellMovieSettings movies;       // +0x18c4
    int unknown_3bc8;                 // +0x3bc8
    int controllerBad;                // +0x3bcc
    signed char badPort;              // +0x3bd0
    signed char activeControllers[4]; // +0x3bd1
    unsigned char unknown_3bd5[3];
    SCoopJoinInfo coopJoin[2];        // +0x3bd8
};
extern ShellManager g_Shell;
#endif
