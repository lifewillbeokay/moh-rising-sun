// PauseMenu.cpp: pausing, restarting and leaving a level.
#include "GameSettingsViews.h"
#include "Shell.h"
extern "C" void *DWI_alloc(const char *, int, int);
void SOUND_PauseAll(bool);
void PlayScriptedSoundFromCode(char *);
void WaitScreen();
inline void *operator new(unsigned int, void *p) { return p; }
extern int g_iWaitCount;
// The field InitStatic sets; only its comparison with 2 is known here.
struct StaticScreenView { unsigned char unknown_0[0x54]; int state; };
extern StaticScreenView g_StaticScreen;
extern IMovingSceneNode *g_pTriggeringPlayer;
class HandlerSaveSlotSelect { public: static void Push(); };
#include "FEngObjects.h"

CPauseMenu::CPauseMenu() {
    unknown_4 = 0;
    unknown_8 = 0;
    initialized = 0;
    paused = 0;
    unknown_14 = 0;
    inMenu = 0;
    wasInMenu = 0;
    unknown_68 = 0;
    unknown_6c = 0;
    unplugged = 0;
    screenName = 0;
    enabled = 0;
    unknown_84 = 0;
}
CPauseMenu::~CPauseMenu() { Shutdown(); }
void CPauseMenu::Shutdown() {
    if (unplugged) delete unplugged;
    unplugged = 0;
    initialized = 0;
}
