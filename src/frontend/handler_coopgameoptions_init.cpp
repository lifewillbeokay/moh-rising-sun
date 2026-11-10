// HandlerCoopGameOptions, the co-op options screen: setup; see docs/Frontend.md.
#include "PackageHandlers.h"
#include "GameSettingsViews.h"
int HandlerCoopGameOptions::Init(const char *) {
    SetScript(package, "MapNumberText", "Show");
    mapNumberText = package->FindObjectByName("MapNumberText");
    MPSetup()->flags &= ~0x00800000;
    MPSetup()->flags &= ~0x20000000;
    MPSetup()->flags &= ~0x10000000;
    MPSetup()->flags |= 0x04000000;
    MPSetup()->flags |= 0x02000000;
    SetOptionsProperties();
    return 0;
}
