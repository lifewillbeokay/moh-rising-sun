// AI-assisted reconstruction from GR8E69; see docs/TriggerObject.md.
#ifndef GAME_LEGACY_FIELD_INFO_H
#define GAME_LEGACY_FIELD_INFO_H

// Sixteen-byte descriptor stride. Member names describe observed uses.
struct LegacyFieldInfo {
    const char *name;
    int type;
    int offset;
    int mask;
};

struct FlexPropList;

#endif
