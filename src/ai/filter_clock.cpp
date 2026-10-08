// AI-assisted reconstruction from GR8E69; see docs/Paths.md.
#include "AIFilterGlobal.h"
extern unsigned int GetFrameCount();
CAIFilterGlobal::CAIFilterGlobal() {
    frameCount = 0;
    time = 0.0f;
    splineManager = 0;
    field10 = 0x2400;
}
void CAIFilterGlobal::UpdateFromGlobals(float delta) {
    frameCount = GetFrameCount();
    time += delta * (1.0f / 60.0f);
}
void CAIFilterGlobal::UpdateToGlobals() {}
