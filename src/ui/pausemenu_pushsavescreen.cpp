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

void CPauseMenu::PushSaveScreen() {
    controllingPlayer = g_pTriggeringPlayer->GetPlayerIndex();
    if (g_bCoopMultiplayer || g_Shell.inCoop) g_Shell.coopPausePlayer = (signed char)controllingPlayer;
    Reset(0, false);
    HandlerSaveSlotSelect::Push();
    BeginSaving();
}
void CPauseMenu::BeginSaving() { inMenu = 1; }
