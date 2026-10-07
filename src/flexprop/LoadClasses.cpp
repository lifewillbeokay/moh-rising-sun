// AI-assisted reconstruction from GR8E69; see docs/FlexProp.md.
#include "database_internal.h"
#include "Endian.h"
#include "StringTable.h"

void FlexPropDatabase::LoadClasses(void *buffer, int count) {
    _STL::map<_STL::string, FlexPropClassFormat *>::iterator it;
    FlexPropClassFormat *record = static_cast<FlexPropClassFormat *>(buffer);
    for (int i = 0; i < count; ++i) {
        ChangeEndian(record->name);
        ChangeEndian(record->recordSize);
        ChangeEndian(record->count);
        ChangeEndian(record->parent);
        for (int j = 0; j < record->count; ++j) {
            ChangeEndian(record->fields[j].type);
            ChangeEndian(record->fields[j].offset);
            ChangeEndian(record->fields[j].crc);
        }
        record->name = g_pStringTable->LookupString(reinterpret_cast<int>(record->name));
        it = g_classes.find(record->name);
        g_classes[record->name] = record;
        record = reinterpret_cast<FlexPropClassFormat *>(
            reinterpret_cast<char *>(record) + record->recordSize);
    }
    for (it = g_classes.begin(); it != g_classes.end(); ++it) {
        FlexPropClassFormat *parentRecord = it->second;
        if (parentRecord->parent) {
            parentRecord->parent = reinterpret_cast<FlexPropClassFormat *>(
                g_pStringTable->LookupString(reinterpret_cast<int>(parentRecord->parent)));
            _STL::map<_STL::string, FlexPropClassFormat *>::iterator parent =
                g_classes.find(reinterpret_cast<char *>(parentRecord->parent));
            parentRecord->parent = parent->second;
        }
    }
}
