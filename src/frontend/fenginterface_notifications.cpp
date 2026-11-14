// AI-assisted reconstruction from GR8E69; see docs/research/frontend-popup-list.md.
#include "FengInterface.h"
#ifdef DECOMP_COMPARE
void CFengInterface::NotificationMessage(unsigned long message, FEObject *object, unsigned long param, unsigned long package) {
    for (CUIObjectSet::iterator it = uiObjects.begin(), end = uiObjects.end(); it != end; ++it)
        (*it)->ProcessUIEvent(message, param, object, (FEPackage *)package);
}
void CFengInterface::NotifySoundMessage(unsigned long message, FEObject *object, unsigned long param, unsigned long package) {
    for (CUIObjectSet::iterator it = uiObjects.begin(), end = uiObjects.end(); it != end; ++it)
        (*it)->ProcessSoundEvent(message, param, object, (FEPackage *)package);
}
#endif
