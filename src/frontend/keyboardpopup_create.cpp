// AI-assisted GR8E69 reconstruction; see docs/research/save-popups.md.
#include "SaveGamePopups.h"
extern unsigned long resetMessage asm("message.3534");
extern int resetMessageGuard asm("_.tmp_0.3535");
extern int g_eResponse;
void CSaveGameKeyboardPopup::CreateName() {
    g_PauseScreen.BeginSaving(); Push();
    if (!resetMessageGuard) { resetMessage = FEHashUpper("Reset"); resetMessageGuard = 1; }
    engine->QueueMessage(resetMessage, 0, 0, 0, 0);
    g_eResponse = 0;
    do { Wait(); } while (!g_eResponse);
    Pop(); g_PauseScreen.inMenu = 0;
}
