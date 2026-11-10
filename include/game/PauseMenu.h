// CPauseMenu: the in-game menu object; see docs/Frontend.md. Class and table names come from symbols,
// member names describe what its methods do with them.
#ifndef GAME_PAUSE_MENU_H
#define GAME_PAUSE_MENU_H
#pragma interface
class FEObject;
class FEPackage;
class CUnplugged {
  public:
    CUnplugged();
    ~CUnplugged();
    void Init();
    enum ePrompt { PromptUnknown0 = 0 }; // Original tag from Display's symbol; values 1, 2 and 4 are observed.
    void Display(int, ePrompt);
    unsigned char unknown_0[8];
};
class CUIObject {
  public:
    virtual ~CUIObject() {} // table at +0
    virtual void ProcessUIEvent(unsigned int, unsigned int, FEObject *, FEPackage *);
    virtual void ProcessSoundEvent(unsigned int, unsigned int, FEObject *, FEPackage *);
};
class CPauseMenu : public CUIObject {
  public:
    int unknown_4;            // +0x04
    int unknown_8;            // +0x08
    int initialized;          // +0x0c
    int paused;               // +0x10
    int unknown_14;           // +0x14
    int inMenu;               // +0x18; name inferred, set by BeginSaving.
    int wasInMenu;            // +0x1c
    unsigned char unknown_20[8];
    unsigned char pauseFlag;  // +0x28
    unsigned char unknown_29[0x68 - 0x29];
    int unknown_68;           // +0x68
    int unknown_6c;           // +0x6c
    CUnplugged *unplugged;    // +0x70
    const char *screenName;   // +0x74
    int restartPending;       // +0x78; set by MPRestartLevel.
    unsigned char controllingPlayer;      // +0x7c
    unsigned char controllerPort; // +0x7d
    int enabled;              // +0x80
    int unknown_84;           // +0x84
    CPauseMenu();
    virtual ~CPauseMenu();
    void Shutdown();
    void Init();
    void Reset(const char *, bool);
    void QuitToShell(bool);
    void RestartLevel();
    void MPRestartLevel();
    void UnpauseGame();
    virtual void ProcessUIEvent(unsigned int, unsigned int, FEObject *, FEPackage *);
    virtual void ProcessSoundEvent(unsigned int, unsigned int, FEObject *, FEPackage *);
    void PushSaveScreen();
    void BeginSaving();
    int UpdateUnplugged();
    int CheckCheatCodes();
    void Update(float);
    void Draw();
    void DrawUnplugged();
};
extern CPauseMenu g_PauseScreen;
#endif
