// HandlerMPProfile's card scan callback and destructor; see docs/Frontend.md.
#include "PackageHandlers.h"
#include <string.h>
void HandlerMPProfile::Execute(const char *name, unsigned int size) {
    if (m_lastProfile < 256) {
        SProfileScanEntry *entry = &m_profiles[m_lastProfile];
        strncpy(entry->name, name, 8);
        entry->name[8] = 0;
        entry->size = size;
        m_lastProfile++;
    }
}
HandlerMPProfile::~HandlerMPProfile() {}
