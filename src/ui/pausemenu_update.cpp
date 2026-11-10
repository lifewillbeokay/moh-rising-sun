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

void CPauseMenu::Update(float elapsed) {
    int pause = UpdateUnplugged();
    if (!unknown_14) pause = 0;
    if (paused) {
        if (pause) pause = CheckCheatCodes() ^ 1;
        if (paused && !pause) {
            Reset(0, false);
            SOUND_PauseAll(false);
        }
    }
    int started = 0;
    if (pause && !paused) {
        Reset(screenName, false);
        SOUND_PauseAll(true);
        unknown_68 = started;
        pauseFlag = started;
        started = 1;
    }
    paused = pause;
    if ((pause || inMenu) && !started) g_Shell.Process((int)(elapsed * 20.0f));
    if (wasInMenu && !inMenu && !paused) Reset(0, false);
    wasInMenu = inMenu;
}
void CPauseMenu::DrawUnplugged() {
    if (initialized && (paused || inMenu) && unknown_6c) {
        int state = g_inputMgr.GetCurrentState(controllerPort);
        int prompt = 1;
        switch (state) {
        case 0:
            prompt = 2;
            break;
        case 1:
            prompt = 4;
            break;
        }
        unplugged->Display((signed char)controllerPort, (CUnplugged::ePrompt)prompt);
    }
}
void CPauseMenu::Draw() {
    if (paused || inMenu) {
        if (!inMenu) {
            FEPackage *screen = (FEPackage *)g_Shell.GetPauseScreen();
            if (screen) {
                FEObject *text = screen->FindObjectByName("Press START Text");
                FEObject *source = screen->FindObjectByName("Press START Text Minus Button Word");
                if (text && source) {
                    ((FEStringObject *)text)->text = (const short *)((FEStringObject *)source)->text.text;
                    text->changed = 1;
                }
            }
        }
        g_Shell.Draw();
        DrawUnplugged();
    }
}
