// AI-assisted GR8E69 reconstruction; see docs/research/save-popups.md.
#include "SaveGamePopups.h"
extern int g_eResponse;
#ifdef DECOMP_COMPARE
void CSaveGameKeyboardPopup::OnUIEvent(unsigned int event, FEObject *) {
    if (event == FEHashUpper("A")) AddToNameString(name, 'A');
    else if (event == FEHashUpper("B")) AddToNameString(name, 'B');
    else if (event == FEHashUpper("C")) AddToNameString(name, 'C');
    else if (event == FEHashUpper("D")) AddToNameString(name, 'D');
    else if (event == FEHashUpper("E")) AddToNameString(name, 'E');
    else if (event == FEHashUpper("F")) AddToNameString(name, 'F');
    else if (event == FEHashUpper("G")) AddToNameString(name, 'G');
    else if (event == FEHashUpper("H")) AddToNameString(name, 'H');
    else if (event == FEHashUpper("I")) AddToNameString(name, 'I');
    else if (event == FEHashUpper("J")) AddToNameString(name, 'J');
    else if (event == FEHashUpper("K")) AddToNameString(name, 'K');
    else if (event == FEHashUpper("L")) AddToNameString(name, 'L');
    else if (event == FEHashUpper("M")) AddToNameString(name, 'M');
    else if (event == FEHashUpper("N")) AddToNameString(name, 'N');
    else if (event == FEHashUpper("O")) AddToNameString(name, 'O');
    else if (event == FEHashUpper("P")) AddToNameString(name, 'P');
    else if (event == FEHashUpper("Q")) AddToNameString(name, 'Q');
    else if (event == FEHashUpper("R")) AddToNameString(name, 'R');
    else if (event == FEHashUpper("S")) AddToNameString(name, 'S');
    else if (event == FEHashUpper("T")) AddToNameString(name, 'T');
    else if (event == FEHashUpper("U")) AddToNameString(name, 'U');
    else if (event == FEHashUpper("V")) AddToNameString(name, 'V');
    else if (event == FEHashUpper("W")) AddToNameString(name, 'W');
    else if (event == FEHashUpper("X")) AddToNameString(name, 'X');
    else if (event == FEHashUpper("Y")) AddToNameString(name, 'Y');
    else if (event == FEHashUpper("Z")) AddToNameString(name, 'Z');
    else if (event == FEHashUpper("1")) AddToNameString(name, '1');
    else if (event == FEHashUpper("2")) AddToNameString(name, '2');
    else if (event == FEHashUpper("3")) AddToNameString(name, '3');
    else if (event == FEHashUpper("4")) AddToNameString(name, '4');
    else if (event == FEHashUpper("5")) AddToNameString(name, '5');
    else if (event == FEHashUpper("6")) AddToNameString(name, '6');
    else if (event == FEHashUpper("7")) AddToNameString(name, '7');
    else if (event == FEHashUpper("8")) AddToNameString(name, '8');
    else if (event == FEHashUpper("9")) AddToNameString(name, '9');
    else if (event == FEHashUpper("0")) AddToNameString(name, '0');
    else if (event == FEHashUpper(".")) AddToNameString(name, '.');
    else if (event == FEHashUpper("@")) AddToNameString(name, '@');
    else if (event == FEHashUpper("Underscore")) AddToNameString(name, '_');
    else if (event == FEHashUpper("Backspace")) BackspaceNameString(name);
    else if (event == FEHashUpper("Clear Profile Name")) name[0] = 0;
    else if (event == FEHashUpper("Text Entry Done")) {
        if (strlen(name)) {
            g_GameSettings.CopyNameIntoSettings(name, 0);
            g_GameSettings.profile->profiles[g_GameSettings.profile->profileIndex[0]].saveNamed = 1;
            g_eResponse = 1;
        }
    } else if (event == FEHashUpper("Text Entry Setup")) {
        nameStarted = 0;
        char buffer[32];
        sprintf(buffer, "PLAYER1_CAPS");
        FEWideString *text = g_Shell.localizer->GetString(buffer, 0);
        sprintf(name, "%s", text->Convert(buffer));
        LocalizeString("DPad text", "CHANGE");
        LocalizeString("X button text", "CONFIRM");
        LocalizeString("Tri button text", "BACK");
        LocalizeString("New Profile Name", "NEW_PROFILE_NAME");
    } else if (event == FEHashUpper("Game Name Aborted")) g_eResponse = 1;
    else printf("Unknown text event\n");
    FEString *string = (FEString *)package->FindObjectByName("Profile Name text");
    string->text = name; string->flags |= 0x400000;
}
#endif
