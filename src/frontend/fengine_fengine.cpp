// FEngine: loading and unloading packages.
#include "FEPackageReader.h"
#include "FEngine.h"
#include "ShellManager.h"
#include <string.h>

FEngine::FEngine() {
    executing = 1;
    resetRequested = 1;
    unknown_5264 = 0;
    joyPadCount = 0;
    activeMask = 0;
    joyPads = 0;
    unknown_44 = 0;
    unknown_40 = 0;
    unknown_e4 = 0;
    unknown_c = 0;
    debugMessages = 0;
    unknown_10 = 1;
    unknown_14 = 1;
    memset(unknown_98, 0, 76);
    markerCount = 0;
    memset(markers, 0, 4096);
    lastButtonIndex = 0;
    memset(lastButtons, 0, 256);
    typeLib.Startup();
    lastAcceptPad = 0;
}
FEngine::~FEngine() { ReleaseAll(); }
