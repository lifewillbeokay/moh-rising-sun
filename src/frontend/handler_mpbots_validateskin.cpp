// HandlerMPBots::ValidateSkin; see docs/Frontend.md.
#include "PackageHandlers.h"
void HandlerMPBots::ValidateSkin(char *list, int team, PackageHandler *handler, int direction) {
    int index = handler->GetObjectListIndex(list);
    while (!IsSkinUnlocked(index, team)) {
        if (direction < 0) {
            handler->SetPrevObjectListIndex(list);
            index = handler->GetObjectListIndex(list);
        } else {
            handler->SetNextObjectListIndex(list);
            index = handler->GetObjectListIndex(list);
        }
    }
}
