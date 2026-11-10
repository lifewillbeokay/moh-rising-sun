// HandlerVirtualKeyboard, the on-screen keyboard: editing its string; see docs/Frontend.md.
#include "PackageHandlers.h"
#include "GameSettings.h"
#include "GameSettingsViews.h"
#include "PauseMenu.h"
#include <string.h>
#include <stdio.h>
void HandlerVirtualKeyboard::ClearStringField() {
    char *string = GetMyString();
    memset(string, 0, GetStringLength());
}
void HandlerVirtualKeyboard::AddLetterToString(char *string, char letter) {
    int length = strlen(string);
    if (length < GetStringLength() - 1) {
        string[length] = letter;
        string[length + 1] = 0;
        g_Shell.PlayShellSound((EShellSoundType)14);
    }
}
void HandlerVirtualKeyboard::RemovePreviousLetter(char *string) {
    int length = strlen(string);
    if (length > 0) {
        g_Shell.PlayShellSound((EShellSoundType)15);
        string[length - 1] = 0;
    }
}
