// AI-assisted reconstruction from GR8E69; see docs/TriggerObject.md.
#include "TriggerObject.h"
bool TriggerObject::HasField(const char *name) const {
    if (flags.flexFormat)
        return properties.flex.IsFieldValid(GetStringCRC(name));
    return true;
}
void TriggerObject::GetPosition(CVector3 &position) const {
    if (flags.flexFormat) {
        properties.flex.GetPosition(position);
    } else {
        xyzProperty_Struct *legacy = properties.legacy;
        position.Set(legacy->field10, legacy->field14, legacy->field18);
    }
}
