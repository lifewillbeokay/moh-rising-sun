// AI-assisted reconstruction from GR8E69; see docs/TriggerObject.md.
#include "LegacyField.h"
#include "LegacyFieldInfo.h"
// Original propdat.cpp storage, scoped by the manifest.
extern LegacyFieldInfo s_coreFields[25];
extern LegacyFieldInfo s_mechanicFields[3];
extern LegacyFieldInfo s_enemyFields[19];
extern LegacyFieldInfo s_powerupFields[1];

template<class T>
void GetLegacyField(ANY_PROP record, const char *name, T &result,
    bool (*read)(void *, LegacyFieldInfo *, int, const char *, T &)) {
    switch (record.xyz->field0c) {
    case 3:
        if (!read(record.xyz, s_enemyFields, 19, name, result))
            read(record.xyz, s_coreFields, 25, name, result);
        break;
    case 2:
        if (!read(record.xyz, s_mechanicFields, 3, name, result))
            read(record.xyz, s_coreFields, 25, name, result);
        break;
    case 4:
        if (!read(record.xyz, s_powerupFields, 1, name, result))
            read(record.xyz, s_coreFields, 25, name, result);
        break;
    case 1: case 5: case 6: case 7: case 8:
    default:
        read(record.xyz, s_coreFields, 25, name, result);
        break;
    }
}
#define INSTANCE(T) \
    template void GetLegacyField<T>(ANY_PROP, const char *, T &, \
        bool (*)(void *, LegacyFieldInfo *, int, const char *, T &));
INSTANCE(int)
INSTANCE(float)
INSTANCE(const char *)
INSTANCE(bool)
INSTANCE(FlexPropList *)

#undef INSTANCE
