// HandlerNameEntry, the name entry screen: setup; see docs/Frontend.md.
#include "PackageHandlers.h"
#include "GameSettings.h"
#include "Shell.h"
#include <stdio.h>
#include <string.h>
int HandlerNameEntry::Init(const char *) {
    switch (package->controlMask) {
    case 1: SetObjectListIndex("Player", 1); break;
    case 2: SetObjectListIndex("Player", 2); break;
    case 4: SetObjectListIndex("Player", 3); break;
    case 8: SetObjectListIndex("Player", 4); break;
    }
    return 0;
}
