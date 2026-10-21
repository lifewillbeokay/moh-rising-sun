// AI-assisted reconstruction from GR8E69; see docs/TriggerObject.md.
#include "TriggerObject.h"
#include "LegacyField.h"

int TriggerObject::GetInt(const char *name) const {
    int result = 0;
    if (flags.flexFormat)
        result = properties.flex.GetInt(name);
    else if (properties.legacy->field08 == 4) {
        ANY_PROP legacy;
        legacy.xyz = properties.legacy;
        ::GetLegacyField(legacy, name, result, GetLegacyFieldInt);
    }
    return result;
}

float TriggerObject::GetFloat(const char *name) const {
    float result = 0.0f;
    if (flags.flexFormat)
        result = properties.flex.GetFloat(name);
    else if (properties.legacy->field08 == 4) {
        ANY_PROP legacy;
        legacy.xyz = properties.legacy;
        ::GetLegacyField(legacy, name, result, GetLegacyFieldFloat);
    }
    return result;
}

const char *TriggerObject::GetString(const char *name) const {
    const char *result = 0;
    if (flags.flexFormat)
        result = properties.flex.GetString(name);
    else if (properties.legacy->field08 == 4) {
        ANY_PROP legacy;
        legacy.xyz = properties.legacy;
        ::GetLegacyField(legacy, name, result, GetLegacyFieldString);
    }
    return result;
}

bool TriggerObject::GetBool(const char *name) const {
    bool result = false;
    if (flags.flexFormat)
        result = properties.flex.GetBool(name);
    else if (properties.legacy->field08 == 4) {
        ANY_PROP legacy;
        legacy.xyz = properties.legacy;
        ::GetLegacyField(legacy, name, result, GetLegacyFieldBool);
    }
    return result;
}
