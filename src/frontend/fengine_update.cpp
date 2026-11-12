// AI-assisted GR8E69 reconstruction; see docs/research/feng-lifecycle.md.
#ifdef DECOMP_COMPARE
#include "FEngine.h"
#include "FEJoyPad.h"
#include "ShellManager.h"
void FEngine::Update(long elapsed) {
    if (debugMessages) game->DebugMessageBeginUpdate();
    if (executing) {
        if (resetRequested) {
            consumedDirections = 0;
            if (unknown_c) {
                FEMouseInfo info; game->GetMouseInfo(info); mouse.Update(info, elapsed);
            }
            for (unsigned char i = 0; i < (unsigned long)joyPadCount; ++i)
                if ((activeMask >> i) & 1) joyPads[i].Update(game->GetJoyPadMask(i), elapsed);
            StartCheckMovieDemoScreen();
            for (FEPackage *package = (FEPackage *)packages.head; package; package = (FEPackage *)package->next) {
                CheckMovieDemoScreen(package->name);
                ProcessPadsForPackage(package);
                if (unknown_c) ProcessMouseForPackage(package);
            }
            unsigned long mask = 1;
            for (unsigned long i = 0; i < 19; ++i, mask <<= 1) {
                if (consumedDirections & mask)
                    for (unsigned char pad = 0; pad < (unsigned long)joyPadCount; ++pad)
                        joyPads[pad].DecrementHold(mask, repeatAmounts[i]);
                repeatAmounts[i] = 0;
            }
            unknown_40 = unknown_44;
        }
        if (executing) {
            do {
                long step = elapsed > 20 ? 20 : elapsed;
                FEPackage::uHoldDirtyFlags = unknown_5264 ? 0 : 0xffffffff;
                elapsed -= step;
                FEPackage *package = (FEPackage *)packages.head;
                while (package) {
                    FEPackage *next = (FEPackage *)package->next;
                    package->Update(this, step); package = next;
                }
                ProcessMessageQueue(); ProcessPackageCommands();
                if (messageQueue.head) ProcessMessageQueue();
                unknown_5264 = 0;
            } while (elapsed);
        } else {
            for (FEPackage *package = (FEPackage *)packages.head; package; package = (FEPackage *)package->next)
                package->Update(this, elapsed);
        }
    } else {
        for (FEPackage *package = (FEPackage *)packages.head; package; package = (FEPackage *)package->next)
            package->Update(this, elapsed);
    }
    if (debugMessages) game->DebugMessageEndUpdate();
}
#endif
