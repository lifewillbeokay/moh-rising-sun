// HandlerPasswordEntry, the password keyboard; see docs/Frontend.md.
#include "PackageHandlers.h"
#include "GameSettings.h"
#include "GameSettingsViews.h"
#include "PauseMenu.h"
int HandlerPasswordEntry::HandleEvent(unsigned int event, FEObject *object, unsigned long param) {
    if (event == FEHashUpper("Text Entry Done")) {
        if (g_Shell.CheckIfValidPassword(text)) g_Shell.PlayShellSound((EShellSoundType)4);
        else g_Shell.PlayShellSound((EShellSoundType)5);
        ClearStringField();
    } else if (event == FEHashUpper("Text Entry Setup")) {
        text[0] = 0;
        cursor = 1;
    }
    HandlerVirtualKeyboard::HandleEvent(event, object, param);
    return 0;
}
char *HandlerPasswordEntry::GetMyString() { return text; }
int HandlerPasswordEntry::GetStringLength() { return 17; }
