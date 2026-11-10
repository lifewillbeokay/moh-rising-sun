// HandlerProfileSelect's constructor; see docs/Frontend.md.
#include "PackageHandlers.h"
#include <stdio.h>
extern CMemoryCard *g_hMemoryCard;
HandlerProfileSelect::HandlerProfileSelect() {
    unknown_58 = 0;
    profileCount = 0;
    for (int i = 0; i < 6; i++) {
        char groupName[40];
        sprintf(groupName, "Profile Group %d", i);
        groupHashes[i] = FEHashUpper(groupName);
    }
    g_hMemoryCard->OnEnterInteractiveMode();
}
