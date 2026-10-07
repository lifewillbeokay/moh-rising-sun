// AI-assisted reconstruction from GR8E69; see docs/TriggerObject.md.
#include "TriggerObject.h"
int LoadPropertyData(xyzProperty_Struct *, int);
int TriggerObject::GetLegacyField(int field) const { return LoadPropertyData(properties.legacy, field); }
const char *TriggerObject::GetClassName() const {
    if (flags.flexFormat)
        return properties.flex.GetClassName();
    return 0;
}
int TriggerObject::IsAnimatedLight() const {
    if (!flags.flexFormat)
        return properties.legacy->field0c == 13;
    return 0;
}
