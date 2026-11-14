// AI-assisted GR8E69 reconstruction; see docs/research/frontend-screens.md.
#include "PackageHandlers.h"
// Symbol-backed private cache storage, retained externally in the original image.
extern unsigned long setupHash asm("uSETUPMENU.3279");
extern int setupGuard asm("_.tmp_0.3280");
extern unsigned long creditsHash asm("uPLAYCREDITS.3281");
extern int creditsGuard asm("_.tmp_1.3282");
int HandlerSpecialFeatures::HandleEvent(unsigned int event, FEObject *object, unsigned long) {
    if (!setupGuard) { setupHash = FEHashUpper("Setup Menu"); setupGuard = 1; }
    if (!creditsGuard) { creditsHash = FEHashUpper("Play Credits Movie"); creditsGuard = 1; }
    if (event == setupHash) SetProfileNameOnScreen(0);
    HandleMovieEvent(event, object); return 0;
}
