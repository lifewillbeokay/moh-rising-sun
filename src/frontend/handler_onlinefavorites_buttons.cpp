// HandlerOnlineFavorites' button helpers; see docs/Frontend.md.
#include "PackageHandlers.h"
void HandlerOnlineFavorites::ToggleButton(unsigned long button, bool enable) {
    if (enable) {
        SetScript(package, button, "Idle");
        EnableButton(package, button);
    } else {
        SetScript(package, button, "Disabled");
        DisableButton(package, button);
    }
}
void HandlerOnlineFavorites::SetButton(unsigned long button) {
    FEObject *object = package->FindObjectByHash(button);
    if (object) {
        package->SetCurrentButton(engine, object, true);
    }
}
