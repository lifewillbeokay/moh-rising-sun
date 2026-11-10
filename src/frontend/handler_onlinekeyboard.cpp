// The online skin search, the online keyboard and HandlerOnlineFavorites' constructor and destructor; see docs/Frontend.md.
#include "PackageHandlers.h"
#include "Shell.h"
#include <string.h>
char HandlerOnlineOptions::FindNextUnlockedSkin(char skin, char step, char first, char end) {
    int unlocked = 0;
    do {
        skin += step;
        if (skin >= end) {
            skin = first;
        } else if (skin < first) {
            skin = end - 1;
        }
        if (IsSkinUnlocked(skin)) {
            unlocked = 1;
        }
    } while (!unlocked);
    return skin;
}
int HandlerOnlineKeyboard::HandleEvent(unsigned int event, FEObject *object, unsigned long param) {
    if (event == FEHashUpper("Text Entry Done")) {
        ShellManager *shellManager = &g_Shell;
        if (!shellManager->keyboardAllowsEmpty) {
            if (strlen(text)) {
                shellManager->SetKeyboardString(text);
                engine->QueueMessage(FEHashUpper("Text Validated"), 0, package, (FEObject *)-3, 0);
                shellManager->PlayShellSound((EShellSoundType)4);
            } else {
                shellManager->PlayShellSound((EShellSoundType)5);
            }
        } else {
            shellManager->SetKeyboardString(text);
            engine->QueueMessage(FEHashUpper("Text Validated"), 0, package, (FEObject *)-3, 0);
            shellManager->PlayShellSound((EShellSoundType)4);
        }
    } else if (event == FEHashUpper("Text Entry Setup")) {
        ShellManager *shellManager = &g_Shell;
        char *dest = text;
        strncpy(dest, shellManager->keyboardString, GetStringLength());
        cursor = 1;
    }
    HandlerVirtualKeyboard::HandleEvent(event, object, param);
    return 0;
}
char *HandlerOnlineKeyboard::GetMyString() { return text; }
int HandlerOnlineKeyboard::GetStringLength() { return 17; }
HandlerOnlineFavorites::HandlerOnlineFavorites() {
    listBox = 0;
    unknown_54 = 0;
}
class ListBox {
  public:
    ~ListBox();
    void Update();
};
HandlerOnlineFavorites::~HandlerOnlineFavorites() {
    if (listBox) {
        delete listBox;
        listBox = 0;
    }
}
