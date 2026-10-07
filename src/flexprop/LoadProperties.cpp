// AI-assisted reconstruction from GR8E69; see docs/FlexProp.md.
#include "database_internal.h"
#include "Endian.h"
#include "StringTable.h"

void FlexPropDatabase::LoadProperties(void *base, void *buffer, int count) {
    _STL::map<_STL::string, FlexPropClassFormat *>::iterator classIt;
    _STL::map<_STL::string, FlexPropFormat *>::iterator propIt;
    char *cursor = static_cast<char *>(buffer);
    for (int i = 0; i < count; ++i) {
        FlexPropFormat *record = reinterpret_cast<FlexPropFormat *>(cursor);
        ChangeEndian(record->name);
        ChangeEndian(record->description);
        ChangeEndian(record->recordSize);
        for (unsigned int j = 0; j < 12; ++j) {
            ChangeEndian(record->transform[j]);
        }
        int *word = record->fields;
        int *end = reinterpret_cast<int *>(
            reinterpret_cast<char *>(record) + record->recordSize);
        for (; word != end; ++word) {
            ChangeEndian(*word);
        }
        record->name = g_pStringTable->LookupString(reinterpret_cast<int>(record->name));
        classIt = g_classes.find(
            g_pStringTable->LookupString(reinterpret_cast<int>(record->description)));
        record->description = classIt->second;
        propIt = g_properties_map.find(record->name);
        g_properties_map[record->name] = record;
        g_properties.push_back(record);
        FlexPropClassFormat *description = record->description;
        while (description) {
            for (int j = 0; j < description->count; ++j) {
                FlexPropField *field = &description->fields[j];
                if (field->type == 6) {
                    int &value = *reinterpret_cast<int *>(
                        reinterpret_cast<char *>(record->fields) + field->offset);
                    value += reinterpret_cast<int>(base);
                }
            }
            description = description->parent;
        }
        cursor += record->recordSize;
    }
    Rewind();
}
