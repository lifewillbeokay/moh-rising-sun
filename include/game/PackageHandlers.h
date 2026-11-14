// The shell's package handlers; scoped views, see docs/Frontend.md.
#ifndef GAME_PACKAGE_HANDLERS_H
#define GAME_PACKAGE_HANDLERS_H
#pragma interface
#include "FEngObjects.h"
#include "FELocalizer.h"
#include "MemoryCard.h"
#include "VolumeKnobs.h"
#include "PauseMenu.h"
#include "FEngStrings.h"
class ShellFactory {
  public:
    ~ShellFactory();
    int Process();
    PackageHandler *CreatePackageHandler(const char *);
    void DestroyPackageHandler(PackageHandler *);
};
// The second table of the handlers that scan memory cards.
struct CMemoryCard::ScanTask {
    virtual ~ScanTask() {}
    virtual void Execute(const char *, unsigned int);
};
// One file a card scan reports. Name inferred; the handlers fill and read a table of these.
struct SProfileScanEntry {
    char name[9];       // +0x0; eight characters and a terminator.
    unsigned int size;  // +0xc
};
class HandlerVirtualKeyboard : public PackageHandler {
  public:
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    int HandleSoundEvent(unsigned int, FEObject *);
    virtual char *GetMyString();
    virtual int GetStringLength();
    void ClearStringField();
    void AddLetterToString(char *, char);
    void RemovePreviousLetter(char *);
};
class HandlerSaveSlotBase : public PackageHandler {
  public:
    HandlerSaveSlotBase();
    ~HandlerSaveSlotBase();
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    int HandleSoundEvent(unsigned int, FEObject *);
    virtual void OnCancel();
    virtual void OnAccept();
    virtual void OnPush();
    virtual void OnPop();
    int GetSelection() const;
    int IsValidLoadGameType(int);
    static void Push(const char *);
    static void Swap(const char *, FEPackage *);
    void Pop();
};
class HandlerControllerOptions : public PackageHandler {
  public:
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    int crosshairs;         // +0x50
    int vibrate;            // +0x54
    int grenadeCook;        // +0x58
    int invertY;            // +0x5c
    float analogSensitivity; // +0x60
    float turnSpeed;        // +0x64
    CSlider sensitivitySlider; // +0x68
    CSlider turnSpeedSlider;   // +0x88
    int player;             // +0xa8
};
class HandlerAudioConfig : public PackageHandler {
  public:
    HandlerAudioConfig() { confirmed = 1; }
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    void Revert();
    float savedSfxVolume;   // +0x50
    float savedMusicVolume; // +0x54
    int soundFormat;        // +0x58
    int subtitles;          // +0x5c
    CVolumeKnobs knobs;     // +0x60
    int confirmed;          // +0xa0; when clear, setting the screen up reverts it.
};
class HandlerCoopProfile : public PackageHandler, public CMemoryCard::ScanTask {
  public:
    ~HandlerCoopProfile();
    int Init(const char *);
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    void Execute(const char *, unsigned int);
    static int m_player1_profile_index;
    static int m_player2_profile_index;
    static int m_create_player1;
    static int m_create_player2;
    static unsigned int m_numProfiles;
    static SProfileScanEntry m_profiles[];
    int selection;   // +0x54; Init sets it to -1. Name inferred.
    int controlMask; // +0x58; the package's mask on Init. Name inferred.
};
class HandlerProfileSelect : public PackageHandler, public CMemoryCard::ScanTask {
  public:
    HandlerProfileSelect();
    static int InSelection();
    static int InKeyboard();
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    void Execute(const char *, unsigned int);
    int profileCount;                // +0x54; the entries Execute has filled.
    int unknown_58;                  // +0x58; the constructor clears it.
    SProfileScanEntry profiles[256]; // +0x5c
    unsigned long groupHashes[6];    // +0x105c; "Profile Group %d".
};
class HandlerMPProfile : public PackageHandler, public CMemoryCard::ScanTask {
  public:
    HandlerMPProfile();
    ~HandlerMPProfile();
    int Init(const char *);
    static int InSelection();
    static int InKeyboard();
    static int m_lastProfile;
    static SProfileScanEntry m_profiles[];
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    void Execute(const char *, unsigned int);
    int unknown_54;               // +0x54; Init clears it.
    int unknown_58;               // +0x58; Init clears it.
    unsigned long groupHashes[2]; // +0x5c; "Profile Group %d".
};
class HandlerQuadrantGroup : public PackageHandler {
  public:
    int Init(const char *);
};
class PackageHandlerMain : public PackageHandler {
  public:
    void HandleTick();
    int HandleEvent(unsigned int, FEObject *, unsigned long);
};
class HandlerNumPlayers : public PackageHandler {
  public:
    int Init(const char *);
    int HandleEvent(unsigned int, FEObject *, unsigned long);
};
class HandlerGameOptions : public PackageHandler {
  public:
    int HandleEvent(unsigned int, FEObject *, unsigned long);
};
class HandlerNameEntry : public HandlerVirtualKeyboard {
  public:
    int Init(const char *);
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    char *GetMyString();
    int GetStringLength();
    int cursor;    // +0x50
    char text[12]; // +0x54
};
class HandlerDifficulty : public PackageHandler {
  public:
    HandlerDifficulty();
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    unsigned long buttonHashes[3]; // +0x50; hashes of the three difficulty buttons' names.
};
enum EShellButtonType { EShellButtonDigital = 0, EShellButtonAnalog = 1, EShellButtonStick = 2 };
// One assignable game function; the type name is from AreButtonsEqual's symbol, the member names are inferred.
struct SFunctionNames {
    int button;            // +0x00; index into the screen's button table.
    int keyIndex;          // +0x04
    const char *name;      // +0x08
    int unknown_c[2];
    EShellButtonType type; // +0x14
};
// A key table entry. Name inferred.
struct SKeyName {
    short key;        // +0x0
    const char *name; // +0x4
};
extern SFunctionNames controllerFunctionNames[];
extern SKeyName controllerKeyNames[];
extern SKeyName analogKeyNames[];
extern SKeyName analogStickNames[];
// Pairs of an analog key and the digital key that counts as the same. Name from its symbol; the member names are inferred.
struct SAnalogEquivalence {
    short analog;  // +0x0
    short digital; // +0x2
};
extern SAnalogEquivalence analogEquivalence[];
class HandlerCustomController : public PackageHandler {
  public:
    HandlerCustomController();
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    int FindKeyIndex(short, EShellButtonType);
    int FindButtonIndex(unsigned int);
    int GetNumKeyNames(EShellButtonType);
    int GetNumAssignableFunctions();
    int AssignToNextKeyIndex(int, int);
    int AreButtonsEqual(SFunctionNames *, SFunctionNames *);
    short buttons[16]; // +0x50; one key per function, -1 when unassigned.
};
class HandlerSplitScreen : public PackageHandler {
  public:
    ~HandlerSplitScreen();
    int Init(const char *);
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    virtual void GotoNextScreen();
    unsigned char unknown_50[0x5c - 0x50];
    int pendingPlayerMask; // +0x5c
};
class HandlerMPGameOptions : public PackageHandler {
  public:
    int Init(const char *);
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    static void BalanceTeam();
    unsigned char unknown_50[92 - 0x50];
};
class HandlerMPGameGlobalOptions : public HandlerMPGameOptions {
  public:
    int Init(const char *);
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    int character; // +0x5c; the profile's character. Name inferred.
};
class HandlerMPJoin : public PackageHandler {
  public:
    int HandleEvent(unsigned int, FEObject *, unsigned long);
};
class HandlerMPWaiting : public PackageHandler {
  public:
    int Init(const char *);
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    static int mbSplitScreenContinueSend;
};
class HandlerMPBots : public PackageHandler {
  public:
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    static int IsSkinUnlocked(int, int);
    static void ValidateSkin(char *, int, PackageHandler *, int);
    unsigned char unknown_50[160 - 0x50];
};
class HandlerMPBotChild : public PackageHandler {
  public:
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    unsigned char unknown_50[96 - 0x50];
};
class HandlerLevelMenu : public PackageHandler {
  public:
    HandlerLevelMenu();
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    int selection; // +0x50; the constructor sets it to -1.
};
class HandlerMainMenu : public PackageHandler {
  public:
    int HandleEvent(unsigned int, FEObject *, unsigned long);
};
class HandlerControllerLayout : public PackageHandler {
  public:
    HandlerControllerLayout();
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    unsigned long buttonHashes[5]; // +0x50; "Preset %d" then "Custom %d".
    unsigned char unknown_64[0x6c - 0x64];
    int unknown_6c;                // +0x6c; the constructor clears it.
};
class HandlerCoopTempLaunch : public PackageHandler {
  public:
    int HandleEvent(unsigned int, FEObject *, unsigned long);
};
class HandlerSpecialFeatures : public PackageHandler {
  public:
    int HandleEvent(unsigned int, FEObject *, unsigned long);
};
class HandlerMovieMenu : public PackageHandler {
  public:
    HandlerMovieMenu();
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    unsigned char unknown_50[0x78 - 0x50];
    unsigned long buttonHashes[11]; // +0x78
};
class HandlerCoop4WaySplit : public PackageHandler {
  public:
    int Init(const char *);
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    virtual void GotoNextScreen();
    int playerCount; // +0x50
    unsigned char unknown_54[96 - 0x54];
};
class HandlerCoopGameOptions : public PackageHandler {
  public:
    static int mSaveSlot;
    int Init(const char *);
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    void SetOptionsProperties();
    int ShowOption(const char **, unsigned int, unsigned int);
    int GetNextEntryInSequence(int *, unsigned int, int, int);
    FEObject *mapNumberText; // +0x50
};
class HandlerLoadGame : public HandlerSaveSlotBase {
  public:
    HandlerLoadGame();
    ~HandlerLoadGame();
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    void OnAccept();
};
class HandlerBonusOptions : public PackageHandler {
  public:
    HandlerBonusOptions();
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    bool GetCheatByIndex(int);
    void SetCheatByIndex(int, bool);
    bool ToggleCheatByIndex(int);
    unsigned long textHashes[12]; // +0x50
};

class HandlerStatsScreen : public PackageHandler {
  public:
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    void CheckToSeeIfSpecialPearlStatsAreNeeded(bool);
    int specialPearlStats; // +0x50
};
class HandlerMapOverlay : public PackageHandler {
  public:
    int HandleEvent(unsigned int, FEObject *, unsigned long);
};
class HandlerMedalsOverlay : public PackageHandler {
  public:
    int HandleEvent(unsigned int, FEObject *, unsigned long);
};
class HandlerPasswordEntry : public HandlerVirtualKeyboard {
  public:
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    char *GetMyString();
    int GetStringLength();
    int cursor;    // +0x50
    char text[20]; // +0x54
};
class HandlerLanguageMenu : public PackageHandler {
  public:
    HandlerLanguageMenu();
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    int selected;     // +0x50; 1-based, as SetObjectListIndex takes it.
    int languages[6]; // +0x54; [0] is unused, the constructor fills 1 to 5 and HandleEvent reads from 1.
};
class HandlerOnlineOptions : public PackageHandler {
  public:
    HandlerOnlineOptions();
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    int IsSkinUnlocked(int);
    char FindNextUnlockedSkin(char, char, char, char);
    unsigned char unknown_50[0x54 - 0x50];
    unsigned long americanHashes[15]; // +0x54; "AmericanA%d".
    unsigned long japaneseHashes[15]; // +0x90; "JapaneseA%d".
};
class HandlerSaveProfile : public PackageHandler {
  public:
    HandlerSaveProfile();
    ~HandlerSaveProfile();
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    int unknown_50;                        // +0x50; the constructor sets it to 1.
    int unknown_54;                        // +0x54; likewise.
    unsigned char unknown_58[92 - 0x58];
};
class HandlerOnlineKeyboard : public HandlerVirtualKeyboard {
  public:
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    char *GetMyString();
    int GetStringLength();
    int cursor;    // +0x50
    char text[20]; // +0x54
};
class ListBox;
class HandlerOnlineFavorites : public PackageHandler {
  public:
    HandlerOnlineFavorites();
    ~HandlerOnlineFavorites();
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    void UpdateFavListBox();
    void RemoveDuplicates();
    void ToggleAddButton(bool, bool);
    void SetFavorite(const char *);
    void ToggleButton(unsigned long, bool);
    void SetButton(unsigned long);
    ListBox *listBox; // +0x50; a scrolling list of favorites. Name inferred.
    int unknown_54;   // +0x54; the constructor clears it.
};
class HandlerOnlineNetConfWarning : public PackageHandler {
  public:
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    void LaunchNetConfigUtility();
};
class HandlerHUDPreferences : public PackageHandler {
  public:
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    unsigned char unknown_50[92 - 0x50];
};
class HandlerPausePreferences : public PackageHandler {
  public:
    int HandleEvent(unsigned int, FEObject *, unsigned long);
};
class FEString;
class CFengInterface;
class CSprite;
class CSaveGamePopup : public CUIObject {
  public:
    static int IsBackgroundEnabled();
    static void EnableBackground();
    static void DisableBackground();
    CSaveGamePopup(const char *);
    virtual ~CSaveGamePopup();
    void Update(float);
    void Draw();
    void ProcessUIEvent(unsigned int, unsigned int, FEObject *, FEPackage *);
    void ProcessSoundEvent(unsigned int, unsigned int, FEObject *, FEPackage *);
    virtual void OnUIEvent(unsigned int, FEObject *);
    virtual void OnSoundEvent(unsigned int, FEObject *);
    virtual void OnPush();
    void Push();
    void Pop();
    void Wait();
    void SetString(class FEString *, const wchar_t unsigned *, bool);
    FEngine *engine;     // +0x04
    CFengInterface *interface; // +0x08
    FEPackage *package;  // +0x0c
    CSprite *sprite;      // +0x10
    int unknown_14;      // +0x14; cleared by construction and Push.
    int displayed;       // +0x18; Display sets it, OnPush clears it.
    unsigned char unknown_1c[4];
};
class HandlerPauseOutro : public PackageHandler {
  public:
    int HandleEvent(unsigned int, FEObject *, unsigned long);
};
class HandlerPauseOutroCoop : public PackageHandler {
  public:
    int HandleEvent(unsigned int, FEObject *, unsigned long);
};
class HandlerPauseBriefingOverlay : public PackageHandler {
  public:
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    int isHistory; // +0x50; which of the two screens is showing.
};
class HandlerSaveSlotSelect : public HandlerSaveSlotBase {
  public:
    HandlerSaveSlotSelect();
    ~HandlerSaveSlotSelect();
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    static void Push();
    void OnCancel();
    void OnAccept();
    void OnPop();
};
class HandlerLoadSlotSelect : public HandlerSaveSlotBase {
  public:
    HandlerLoadSlotSelect();
    ~HandlerLoadSlotSelect();
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    void OnCancel();
    void OnAccept();
};
class HandlerLeaderboard : public PackageHandler {
  public:
    int HandleEvent(unsigned int, FEObject *, unsigned long);
    void fillNumString(const char *, int, bool);
    void fillNumGroupString(FEGroup *, const char *, int, bool);
    unsigned char unknown_50[84 - 0x50];
};
class Handler_2P_Menu : public PackageHandler {
  public:
    int HandleEvent(unsigned int, FEObject *, unsigned long);
};
#endif
