// HandlerOnlineOptions' constructor; see docs/Frontend.md.
#include "PackageHandlers.h"
#include <stdio.h>
HandlerOnlineOptions::HandlerOnlineOptions() {
    char name[64];
    for (int i = 0; i < 15; i++) {
        sprintf(name, "AmericanA%d", i + 1);
        americanHashes[i] = FEHashUpper(name);
        sprintf(name, "JapaneseA%d", i + 1);
        japaneseHashes[i] = FEHashUpper(name);
    }
}
