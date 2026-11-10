// CGameSettings: the settings singleton and its saved data.
#include "GameSettingsViews.h"

void CGameSettings::Validate() {}
void CGameSettings::SerializeValue(unsigned int &value, unsigned int bits) {
    if (serializeMode == 1 || serializeMode == 3) profile->saveData.buffer.Save(value, bits);
    else if (serializeMode == 2) value = profile->saveData.buffer.Load(bits);
}
void CGameSettings::SerializeValue(int &value, unsigned int bits) { SerializeValue((unsigned int &)value, bits); }
void CGameSettings::SerializeValue(unsigned char &value) {
    unsigned int temp = value;
    unsigned int &ref = temp;
    SerializeValue(ref, 9);
    value = ref;
}
void CGameSettings::SerializeValue(bool &value) {
    if (serializeMode == 1 || serializeMode == 3) profile->saveData.buffer.Save(value, 1);
    else if (serializeMode == 2) value = profile->saveData.buffer.Load(1) != 0;
}
void CGameSettings::SerializeValue(float &value) { SerializeValue((unsigned int &)value, 32); }
void CGameSettings::SerializeValue(CVector3 &v) {
    SerializeValue(v.x);
    SerializeValue(v.y);
    SerializeValue(v.z);
}
void CGameSettings::SerializeValue(CQuaternion &q) {
    SerializeValue(q.x);
    SerializeValue(q.y);
    SerializeValue(q.z);
    SerializeValue(q.w);
}
