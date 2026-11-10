// HandlerLanguageMenu, the language menu; see docs/Frontend.md.
#include "PackageHandlers.h"
#include "GameSettings.h"
HandlerLanguageMenu::HandlerLanguageMenu() {
    languages[1] = 1;
    languages[4] = 6;
    languages[5] = 7;
    languages[3] = 3;
    languages[2] = 2;
}
int HandlerLanguageMenu::HandleEvent(unsigned int event, FEObject *, unsigned long) {
    if (event == FEHashUpper("Setup Menu")) {
        int language = g_GameSettings.profile->language;
        int i;
        for (i = 1; i <= 5; i++) {
            if (languages[i] == language) break;
        }
        if (i > 5) selected = 1;
        else selected = i;
        SetObjectListIndex("LanguageList", selected);
    } else if (event == FEHashUpper("Pad_Left")) {
        if (--selected <= 0) selected = 5;
        SetObjectListIndex("LanguageList", selected);
        g_GameSettings.Set_Language((ELanguages)languages[selected]);
    } else if (event == FEHashUpper("Pad_Right")) {
        if (++selected > 5) selected = 1;
        SetObjectListIndex("LanguageList", selected);
        g_GameSettings.Set_Language((ELanguages)languages[selected]);
    }
    return 0;
}
