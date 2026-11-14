// AI-assisted reconstruction from GR8E69; see docs/research/frontend-popup-list.md.
#include "PackageHandlers.h"
void CSaveGamePopup::ProcessUIEvent(unsigned int event, unsigned int, FEObject *object, FEPackage *) {
    OnUIEvent(event, object);
}
void CSaveGamePopup::ProcessSoundEvent(unsigned int event, unsigned int, FEObject *object, FEPackage *) {
    OnSoundEvent(event, object);
}
