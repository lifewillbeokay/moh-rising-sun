// HandlerCoopProfile's card scan callback; see docs/Frontend.md.
#include "PackageHandlers.h"
#include <string.h>
void HandlerCoopProfile::Execute(const char *name, unsigned int size) {
    if (m_numProfiles < 256) {
        SProfileScanEntry *entry = &m_profiles[m_numProfiles];
        strncpy(entry->name, name, 8);
        entry->size = size;
        m_numProfiles++;
    }
}
