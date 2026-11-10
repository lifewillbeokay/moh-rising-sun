// HandlerProfileSelect's card scan callback; see docs/Frontend.md.
#include "PackageHandlers.h"
#include <string.h>
void HandlerProfileSelect::Execute(const char *name, unsigned int size) {
    if (profileCount < 256) {
        SProfileScanEntry *entry = &profiles[profileCount];
        strncpy(entry->name, name, 8);
        entry->size = size;
        entry->name[8] = 0;
        profileCount++;
    }
}
