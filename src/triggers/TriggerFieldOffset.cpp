// AI-assisted reconstruction from GR8E69; see docs/TriggerObject.md.
#include "TriggerObject.h"
int TriggerObject::GetFieldOffset(const char *name) const { return properties.flex.GetFieldOffset(GetStringCRC(name)); }
void *TriggerObject::GetData(int offset) const { return properties.flex.GetData(offset); }
