// AI-assisted interface reconstruction from GR8E69; see docs/TriggerObject.md.
#ifndef GAME_LEGACY_FIELD_H
#define GAME_LEGACY_FIELD_H
#include "BPD.h"

struct LegacyFieldInfo;
struct FlexPropList;

// Original aggregate name from the template symbols. Only its observed,
// four-byte legacy pointer argument is modeled; other union views are unknown.
union ANY_PROP {
    xyzProperty_Struct *xyz; // Descriptive member name.
};

// Reconstructed lookup templates and private propdat.cpp callbacks.
// These declarations bridge separately verified fragments.
template<class T>
void GetLegacyField(ANY_PROP, const char *, T &,
    bool (*)(void *, LegacyFieldInfo *, int, const char *, T &));

bool GetLegacyFieldInt(void *, LegacyFieldInfo *, int, const char *, int &);
bool GetLegacyFieldFloat(void *, LegacyFieldInfo *, int, const char *, float &);
bool GetLegacyFieldString(void *, LegacyFieldInfo *, int, const char *, const char *&);
bool GetLegacyFieldBool(void *, LegacyFieldInfo *, int, const char *, bool &);
bool GetLegacyFieldList(void *, LegacyFieldInfo *, int, const char *, FlexPropList *&);

#endif
