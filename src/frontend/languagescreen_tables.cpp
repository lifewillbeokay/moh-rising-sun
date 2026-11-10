// CSaveGameLanguageScreen's language tables; see docs/Frontend.md.
#include "SaveGamePopups.h"
void CSaveGameLanguageScreen::OnSoundEvent(unsigned int, FEObject *) {}
int CSaveGameLanguageScreen::GetLocaleID(int button) {
    switch (languages[button]) {
    case 7:
        return 4;
    case 8:
        return 5;
    case 2:
        return 1;
    case 3:
        return 2;
    case 6:
        return 3;
    case 0:
    default:
        return 0;
    }
}
int CSaveGameLanguageScreen::GetButtonName(int button) {
    switch (languages[button]) {
    case 7:
        return 6;
    case 8:
        return 7;
    case 2:
        return 8;
    case 3:
        return 5;
    case 6:
        return 4;
    case 0:
    default:
        return 3;
    }
}
