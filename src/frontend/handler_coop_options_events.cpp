// AI-assisted GR8E69 reconstruction; see docs/research/frontend-screens.md.
#ifdef DECOMP_COMPARE
#include "PackageHandlers.h"
#include "Shell.h"
#include "GameSettings.h"
#include <stdio.h>
enum GcSwapDiscsMode { GcSwapDiscsMode0 = 0 };
int LevelExistsOnCurrentDisc(int, int);
int SwapDiscs(GcSwapDiscsMode);
extern unsigned long enableHash asm("sEnableInputs.3318");
extern int enableGuard asm("_.tmp_0.3319");
extern int splitSequence[2] asm("splitSequence.3320");
int HandlerCoopGameOptions::HandleEvent(unsigned int event, FEObject *, unsigned long) {
    int direction = 0; unsigned int sound = 17;
    if (!enableGuard) { enableHash = FEHashUpper("Enable_All_Inputs"); enableGuard = 1; }
    if (event == FEHashUpper("Start Coop Multiplayer")) {
        int failed = 0;
        if (!LevelExistsOnCurrentDisc(5, g_GameSettings.nextLevel)) failed = SwapDiscs((GcSwapDiscsMode)1);
        if (!failed) {
            if (mSaveSlot != -1) g_GameSettings.RestoreSaveData(mSaveSlot, g_Shell.inCoop ? g_Shell.coopPausePlayer : 0);
            g_Shell.SetStartMPDataFromShell();
        } else engine->QueueMessage(enableHash, 0, 0, (FEObject *)-2, 0);
    }
    if (event == FEHashUpper("Pad_Left")) direction = -1;
    else if (event == FEHashUpper("Pad_Right")) direction = 1;
    else if (event == FEHashUpper("Pad_Back")) {
        mSaveSlot = -1; engine->QueueMessage(FEHashUpper("Go Back"), 0, 0, (FEObject *)-2, 0);
    }
    if (direction) {
        FEObject *button = package->currentButton;
        if (button->hash != FEHashUpper("MapSelectionButton")) {
            if (button->hash == FEHashUpper("FFSelectionButton")) {
                g_GameSettings.profile->field_32074 ^= 0x80000000;
                sound = (unsigned int)g_GameSettings.profile->field_32074 >> 31;
            } else if (button->hash == FEHashUpper("SplitButton")) {
                unsigned int split = (g_GameSettings.profile->field_32074 >> 28) & 1;
                if (g_GameSettings.profile->field_32074 & 0x20000000) split = 2;
                int next = GetNextEntryInSequence(splitSequence, 2, direction, split);
                g_GameSettings.profile->field_32074 = (g_GameSettings.profile->field_32074 & ~0x20000000) | ((next == 2) << 29);
                g_GameSettings.profile->field_32074 = (g_GameSettings.profile->field_32074 & ~0x10000000) | ((next == 1) << 28);
                sound = (g_GameSettings.profile->field_32074 >> 28) & 1;
            } else if (button->hash == FEHashUpper("ShareHealthButton")) {
                g_GameSettings.profile->field_32074 ^= 0x04000000;
                sound = (g_GameSettings.profile->field_32074 >> 26) & 1;
            } else if (button->hash == FEHashUpper("ShareAmmoButton")) {
                g_GameSettings.profile->field_32074 ^= 0x02000000;
                sound = (g_GameSettings.profile->field_32074 >> 25) & 1;
            } else printf("Unknown controller option '%s'!\n", button->name);
        }
    }
    if (sound != 17) g_Shell.PlayShellSound((EShellSoundType)sound);
    SetOptionsProperties(); return 0;
}
#endif
