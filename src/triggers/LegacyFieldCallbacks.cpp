// AI-assisted reconstruction from GR8E69; see docs/TriggerObject.md.
#include "LegacyFieldInfo.h"
extern "C" int strcasecmp(const char *, const char *);
void DebugMsg(const char *, ...);
// Shared original diagnostic; this fragment owns its complete storage.
static const char invalidFieldType[]
    __attribute__((section(".rodata.legacy_diagnostic"), aligned(4))) = {
    'I', 'n', 'v', 'a', 'l', 'i', 'd', ' ', 'F', 'i', 'e', 'l', 'd', ' ', 't', 'y', 'p', 'e', '.', '\0'
};

static bool GetLegacyFieldBool(void *record, LegacyFieldInfo *fields, int count,
    const char *name, bool &result) {
    for (int i = 0; i < count; ++i) {
        if (strcasecmp(name, fields[i].name) == 0) {
            signed char value = *((signed char *)record + fields[i].offset);
            switch (fields[i].type) {
            case 0: result = (value & fields[i].mask) != 0; return true;
            case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8:
            default: result = false; DebugMsg(invalidFieldType); return true;
            }
        }
    }
    return false;
}

static bool GetLegacyFieldInt(void *record, LegacyFieldInfo *fields, int count,
    const char *name, int &result) {
    for (int i = 0; i < count; ++i) {
        if (strcasecmp(name, fields[i].name) == 0) {
            char *value = (char *)record + fields[i].offset;
            switch (fields[i].type) {
            case 1: result = *(signed char *)value; return true;
            case 2: result = *(short *)value; return true;
            case 3: case 4: result = *(int *)value; return true;
            case 0: case 5: case 6: case 7: case 8:
            default: result = 0; DebugMsg(invalidFieldType); return true;
            }
        }
    }
    return false;
}

static bool GetLegacyFieldFloat(void *record, LegacyFieldInfo *fields, int count,
    const char *name, float &result) {
    for (int i = 0; i < count; ++i) {
        if (strcasecmp(name, fields[i].name) == 0) {
            char *value = (char *)record + fields[i].offset;
            switch (fields[i].type) {
            case 5: result = *(float *)value; return true;
            case 6: result = *(short *)value * 0.0625f; return true;
            case 0: case 1: case 2: case 3: case 4: case 7: case 8:
            default: result = 0.0f; DebugMsg(invalidFieldType); return true;
            }
        }
    }
    return false;
}

static bool GetLegacyFieldString(void *record, LegacyFieldInfo *fields, int count,
    const char *name, const char *&result) {
    for (int i = 0; i < count; ++i) {
        if (strcasecmp(name, fields[i].name) == 0) {
            char *value = (char *)record + fields[i].offset;
            switch (fields[i].type) {
            case 7: result = *(const char **)value; return true;
            case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 8:
            default: result = 0; DebugMsg(invalidFieldType); return true;
            }
        }
    }
    return false;
}

static bool GetLegacyFieldList(void *record, LegacyFieldInfo *fields, int count,
    const char *name, FlexPropList *&result) {
    for (int i = 0; i < count; ++i) {
        if (strcasecmp(name, fields[i].name) == 0) {
            char *value = (char *)record + fields[i].offset;
            switch (fields[i].type) {
            case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7:
            default: break;
            case 8: result = *(FlexPropList **)value; return true;
            }
            result = 0;
            DebugMsg(invalidFieldType);
            return true;
        }
    }
    return false;
}
