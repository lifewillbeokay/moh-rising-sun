// AI-assisted reconstruction from GR8E69; see docs/TriggerObject.md.
#include "TriggerObject.h"

// Descriptive storage annotations for property names shared with original code.
static const char bindingName[]
    __attribute__((section(".rodata.binding_name"), aligned(4))) =
    { 'e', 'R', 'u', 'n', 't', 'i', 'm', 'e', 'B', 'i', 'n', 'd', 'i', 'n', 'g', 0 };
static const char childTypeName[]
    __attribute__((section(".rodata.child_type"), aligned(4))) =
    { 'C', 'h', 'i', 'l', 'd', 'T', 'y', 'p', 'e', 0 };
static const char idName[]
    __attribute__((section(".rodata.id_name"), aligned(4))) = { 'I', 'D', 0 };

int TriggerObject::GetNativeBindingType() const {
    if (flags.flexFormat) {
        if (properties.flex.IsFieldValid(bindingName))
            return properties.flex.GetEnum(bindingName);
        return 0;
    }
    switch (properties.legacy->field0c) {
    case 0: return 0;
    case 1: return 0;
    case 2: return 1;
    case 8: return 2;
    case 3: return 3;
    case 4: return 4;
    case 5: return 5;
    case 6: return 0;
    case 7: return 0;
    case 13: return 6;
    default: return 0;
    }
}

int TriggerObject::GetChildType() const {
    if (flags.flexFormat)
        return GetEnum(childTypeName);
    return ((MOH_core_Struct *)properties.legacy)->field48;
}

int TriggerObject::GetID() const {
    if (flags.flexFormat)
        return properties.flex.GetInt(idName);
    return ((MOH_core_Struct *)properties.legacy)->field38;
}

const char *TriggerObject::GetScript() const {
    if (flags.flexFormat) {
        if (properties.flex.GetFieldType("TriggerClass") == 2)
            return properties.flex.GetString("TriggerClass");
        return properties.flex.GetClassName();
    }
    return ((MOH_core_Struct *)properties.legacy)->field40;
}

const char *TriggerObject::GetChildScript() const {
    if (flags.flexFormat) {
        if (properties.flex.GetFieldType("TriggerClass") == 2)
            return properties.flex.GetClassName();
        return 0;
    }
    return ((MOH_core_Struct *)properties.legacy)->field4c;
}
