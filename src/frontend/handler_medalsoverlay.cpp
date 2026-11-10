// HandlerMedalsOverlay, the medals screen; see docs/Frontend.md.
#include "PackageHandlers.h"
#include "GameSettings.h"
#include <stdio.h>
int HandlerMedalsOverlay::HandleEvent(unsigned int event, FEObject *, unsigned long) {
    if (event == FEHashUpper("Get Medal Status")) {
        char name[16];
        SetProfileNameOnScreen(0);
        int none = 1;
        for (int i = 1; i <= 11; i++) {
            sprintf(name, "Medal%.2d", i);
            if (g_GameSettings.HasMedal(i)) {
                none = 0;
                SetScript(package, name, "Show");
            } else {
                SetScript(package, name, "Hide");
            }
        }
        if (none) SetScript(package, "No Medals Text", "Show");
        else SetScript(package, "No Medals Text", "Hide");
    }
    return 0;
}
