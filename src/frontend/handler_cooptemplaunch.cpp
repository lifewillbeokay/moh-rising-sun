// HandlerCoopTempLaunch, the co-op launch screen; see docs/Frontend.md.
#include "PackageHandlers.h"
int HandlerCoopTempLaunch::HandleEvent(unsigned int event, FEObject *object, unsigned long) {
    HandleCoopLevelStartEvent(event, object);
    return 0;
}
