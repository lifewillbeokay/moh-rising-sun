// AI-assisted reconstruction from GR8E69; see docs/TriggerObject.md.
#include "TriggerObject.h"
#include "LegacyField.h"
#include "StringCRC.h"

void GetBaseName(const char *, char *);

int TriggerObject::GetEnum(const char *name) const {
    int result = 0;
    if (flags.flexFormat) {
        if (properties.flex.GetFieldType(name) == 2) {
            result = GetStringCRC(properties.flex.GetString(name));
        } else if (properties.flex.GetFieldType(name) == 8) {
            char base[256];
            GetBaseName(properties.flex.GetString(name), base);
            result = GetStringCRC(base);
        } else {
            result = properties.flex.GetEnum(name);
        }
    } else if (properties.legacy->field08 == 4) {
        ANY_PROP legacy;
        legacy.xyz = properties.legacy;
        ::GetLegacyField(legacy, name, result, GetLegacyFieldInt);
    }
    return result;
}

FlexPropList *TriggerObject::GetList(const char *name) const {
    FlexPropList *result = 0;
    if (flags.flexFormat)
        result = properties.flex.GetList(name);
    else if (properties.legacy->field08 == 4) {
        ANY_PROP legacy;
        legacy.xyz = properties.legacy;
        ::GetLegacyField(legacy, name, result, GetLegacyFieldList);
    }
    return result;
}
