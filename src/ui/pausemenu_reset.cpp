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

void CPauseMenu::Reset(const char *screen, bool all) {
    if (all) {
        for (int i = 0; i < g_NumPlayers; i++) g_Shell.engine->activeMask |= 1 << (unsigned char)i;
    } else {
        for (int i = 0; i < g_NumPlayers; i++) g_Shell.engine->activeMask &= ~(1 << (unsigned char)i);
        g_Shell.engine->activeMask |= 1 << controllingPlayer;
    }
    g_Shell.Reset(screen);
}
void CPauseMenu::QuitToShell(bool keep) {
    paused = 0;
    g_GameFlow.startingMP = 1;
    g_GameSettings.profile->saveData.quitFlag = keep;
    g_GameSettings.SetPrevMissionAndLevel(g_GameSettings.profile->saveData.mission, g_GameSettings.profile->saveData.level);
    g_GameSettings.SetNextMissionAndLevel(1, 16, 0);
}
void CPauseMenu::RestartLevel() {
    paused = 0;
    g_GameSettings.RestoreLevelStartData();
    g_GameFlow.levelEnding = 1;
}
void CPauseMenu::MPRestartLevel() {
    int one = 1;
    paused = 0;
    restartPending = one;
    g_GameSettings.RestoreLevelStartData();
    g_GameFlow.levelEnding = one;
}
void CPauseMenu::UnpauseGame() {
    paused = 0;
    SOUND_PauseAll(false);
}
