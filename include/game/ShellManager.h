// ShellManager: the front end's connection to FEng. Scoped view; see docs/Frontend.md.
// Only the members and virtual slots reconstructed code touches are named.
#ifndef GAME_SHELL_MANAGER_H
#define GAME_SHELL_MANAGER_H
#pragma interface
#include "ConfigFile.h"
#include "FEngLists.h"
#include "FEngine.h"
#include "FEngRender.h"

class FEMatrix4;
struct FEObjData;
struct FEObjectListEntry;
struct FEMouseInfo;
class ShellLoader {
  public:
    ~ShellLoader();
    int Process();
    int LoadPackage(const char *, unsigned char **, bool &);
    int LoadResources(FEPackage *, long, FEResourceRequest *);
    int UnloadResources(FEPackage *, long, FEResourceRequest *);
};
enum EShellSoundType { EShellSoundType0 = 0 };
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
enum FEng_WarningLevel { FEng_WarningLevel0 = 0 };
class FECodeListBox;
class FEMouse;
class FEGameInterface {
  public:
    virtual int GetPackageData(const char *, unsigned char **, bool &);
    virtual int LocalizePackage(FEPackage *);
    virtual int LoadResources(FEPackage *, long, FEResourceRequest *);
    virtual int UnloadResources(FEPackage *, long, FEResourceRequest *);
    virtual void PackageWasLoaded(FEPackage *);
    virtual int PackageWillUnload(FEPackage *);
    virtual int UnloadUnreferencedLibrary(FEPackage *);
    virtual void NotificationMessage(unsigned long, FEObject *, unsigned long, unsigned long);
    virtual void NotifySoundMessage(unsigned long, FEObject *, unsigned long, unsigned long);
    virtual void BeginPackageRendering(FEPackage *);
    virtual void EndPackageRendering(FEPackage *);
    virtual void GenerateRenderContext(unsigned short, FEObject *);
    virtual int GetContextTransform(unsigned short, FEMatrix4 &);
    virtual void RenderObjectList(FEObjectListEntry *, unsigned long);
    virtual void RenderObject(FEObject *);
    virtual void DrawMousePointer(FEMouse &);
    virtual void GetViewTransformation(FEMatrix4 *);
    virtual unsigned long GetJoyPadMask(unsigned char);
    virtual void GetMouseInfo(FEMouseInfo &);
    virtual int DoesPointTouchObject(float, float, FEObject *);
    virtual void SetCellData(FECodeListBox *, unsigned long, unsigned long);
    virtual void OutputWarning(const char *, FEng_WarningLevel);
    virtual void DebugMessageQueued(unsigned long, FEObject *, FEPackage *, FEObject *, unsigned long);
    virtual void DebugMessageProcessed(unsigned long, FEObject *, FEObject *, FEPackage *, unsigned long);
    virtual void DebugMessageBeginUpdate();
    virtual void DebugMessageEndUpdate();
    virtual ~FEGameInterface();
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
    void GenerateXformMatrix(EAGL::Transform *, FEObjData *, unsigned short);
    void GenerateRenderContext(unsigned short, FEObject *);
    int GetContextTransform(unsigned short, FEMatrix4 &);
    void RenderObject(FEObject *);
    void RenderImage(class FEImage *);
    void RenderString(class FEString *);
    void RenderSimpleImage(class FESimpleImage *);
    unsigned long GetJoyPadMask(unsigned char);
    int InitLocalizer(const char *, const char *);
    void MoveShellCamera();
    unsigned long GetCombinedJoyPadActiveMask() const;
    void GetMouseInfo(FEMouseInfo &);
    int CheckIfValidPassword(char *);
    void DoCoopUnjoin(FEPackage *, signed char);
    void PlayShellSound(EShellSoundType);
    void SetMovieName(int, char *);
    void PlayMovie(_ShellMoviePaths *, bool);
    void PlayMovie(EMovieType, int, bool);
    FENode *GetPauseScreen();
    void SetNumberJoyPads(unsigned char);
    int LocalizePackage(FEPackage *);
    int LocalizeObjects(FEObject *);
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
    int Shutdown();
    void DoMPJoinScreenUpdate();
    void DoCoopJoinScreenUpdate();
    void DoCoopPackageControlMaskUpdating();
    void DoPackageControlMaskUpdating();
    void PerformPostDrawUpdates();
    void NotificationMessage(unsigned long, FEObject *, unsigned long, unsigned long);
    void NotifySoundMessage(unsigned long, FEObject *, unsigned long, unsigned long);
    void PackageWasLoaded(FEPackage *);
    int PackageWillUnload(FEPackage *);
    void ResetCoopJoinInfoFromPortNum(unsigned char);
    void DoCoopControllerChecks();
    int IsCoopOnTop(unsigned char);
    int IsCoopOnTopFromPortNum(unsigned char);
    signed char GetDeviceIndexFromControlMask(unsigned long);
    signed char GetPortNumFromControlMask(unsigned long);
    void SetKeyboardString(const char *);
    unsigned char GetNumCoopPlayers();
    signed char GetIndexPadLastPressedAccept();

    char keyboardString[0x14];        // +0x04; the extent past the 17 bytes copied is unknown.
    int keyboardAllowsEmpty;          // +0x18; HandlerOnlineKeyboard refuses an empty string while clear. Name inferred.
    unsigned char unknown_1c[8];
    int selectedLevel;                // +0x24; CGameSettings reads per-level stats at this - 1.
#ifdef GAME_SHELL_HANDLER_MAP_H
    ShellHandlerMap handlers;         // +0x28
#else
    unsigned char handlers[0x10];     // +0x28; the map, for units without the container headers. See ShellHandlerMap.h.
#endif
    class ShellFactory *factory;      // +0x38
    ShellLoader *loader;              // +0x3c
    FEngine *engine;                  // +0x40
    class CUnplugged *unplugged;      // +0x44
    unsigned char unknown_48[4];
    class FELocalizer *localizer;     // +0x4c
    unsigned char unknown_50[0x60 - 0x50];
    EAGL::Transform viewTransform;    // +0x60
    SFengRenderContext contexts[64];  // +0xa0
    unsigned char mpPlayerCount;      // +0x18a0; copied to the settings when the match starts. Name inferred.
    unsigned char coopJoinBits;       // +0x18a1; bit n per player.
    unsigned char unknown_18a2[0x18a8 - 0x18a2];
    int coopPausePlayer;              // +0x18a8; set when a co-op game opens the save screen. Name inferred.
    int lastStickX;                   // +0x18ac; the stick's last value in GetJoyPadMask.
    int lastStickY;                   // +0x18b0
    int updateOnce;                   // +0x18b4; Process runs one engine update alone while it is set. Name inferred.
    int inCoop;                       // +0x18b8; UpdateUnplugged picks the co-op controller checks.
    int inMultiplayer;                // +0x18bc; UpdateUnplugged picks the multiplayer checks.
    int unknown_18c0;                 // +0x18c0; set by "Start Online Multiplayer".

    CShellMovieSettings movies;       // +0x18c4
    int unknown_3bc8;                 // +0x3bc8; tested by HandlerMPWaiting::Init, set by the player-count menu.
    int controllerBad;                // +0x3bcc
    signed char badPort;              // +0x3bd0
    signed char activeControllers[4]; // +0x3bd1
    unsigned char unknown_3bd5[3];
    SCoopJoinInfo coopJoin[2];        // +0x3bd8
    int quadrantUsed[2];              // +0x3bf0; tested by HandlerQuadrantGroup::Init. Name inferred.
    int unknown_3bf8[2];              // +0x3bf8; cleared together on leaving the outro; the historical movie is skipped while either is set.
    int savedProfilesCRC;             // +0x3c00; HandlerGameOptions compares it with GetProfilesCRCValue. Name inferred.
};
extern ShellManager g_Shell;
#endif
