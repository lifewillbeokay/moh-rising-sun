// AI-assisted GR8E69 reconstruction; see docs/research/feng-packages.md.
#ifdef DECOMP_COMPARE
#include "FEngine.h"
#include "FEngObjects.h"
#include "ShellManager.h"
#include <string.h>
// The same trivial element view used by SetNumJoyPads; no element destructor.
class FEJoyPad { public: FEJoyPad(); unsigned char state[136]; };
void FEngine::ReleaseAll() {
    FEPackage *package;
    while ((package = (FEPackage *)packages.head) != 0) {
        package->Shutdown(game);
        ((FEPackageList *)&packages)->RemovePackage(package);
        delete package;
    }
    FEMinNode *node;
    while ((node = idlePackages.RemHead()) != 0) delete node;
    while ((node = libraries.RemHead()) != 0) delete node;
    while ((node = messageQueue.RemHead()) != 0) delete node;
    if (joyPads) delete[] joyPads;
    joyPads = 0;
    joyPadCount = 0;
    unknown_5264 = unknown_44 = unknown_40 = 0;
    markerCount = 0;
    memset(markers, 0, sizeof(markers));
    lastButtonIndex = 0;
    memset(lastButtons, 0, sizeof(lastButtons));
    while ((node = typeLib.types.RemHead()) != 0) delete node;
}
#endif
