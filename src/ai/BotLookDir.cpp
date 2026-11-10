// AI-assisted reconstruction from GR8E69. A fragment of the original bot object source: the
// original file name is not known. Method names come from the symbol table.
#include <stdlib.h>
#include "AIBot.h"
#include "MathFun.h"

void CBotObject::UpdateLookDir() {
    static int dLookIndex = 0;
    static float ftLookUpdateClock = 0.0f;
    if (ftLookUpdateClock == 0.0f) {
        ftLookUpdateClock = GetClock() + 1.0f;
        if (threatCount > 0) {
            SetLookTarget(threatPositions[dLookIndex]);
            dLookIndex++;
            dLookIndex = dLookIndex < threatCount ? dLookIndex : 0;
        }
    } else if (!(GetClock() <= ftLookUpdateClock)) {
        ftLookUpdateClock = 0.0f;
    }
}
