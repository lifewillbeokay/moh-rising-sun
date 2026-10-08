// AI-assisted reconstruction from GR8E69; see docs/Targeting.md.
#include "AITargeting.h"
#include "AIFilterGlobal.h"
bool CAITargeting::UpdateReactionTime() {
    if (!target.HasTarget()) {
        if (!(lastSeenTime + 3.0f <= g_aigAIFilterGlobalObject.time))
            lastSeenSet = 0;
    } else if (lastSeenSet) {
        lastSeenTime = g_aigAIFilterGlobalObject.time;
    }
    return true;
}
