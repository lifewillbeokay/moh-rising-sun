// A script-registered location an AI object can target. The tag and ETeam come from the symbol table; field names are
// inferred from CAITargeting::RegisterLocationTarget.
#ifndef GAME_LOCATION_TARGET_INFO_H
#define GAME_LOCATION_TARGET_INFO_H
struct BSObject;
enum ETeam { TeamUnknown = 0 };
struct _LocationTargetInfo {
    bool enabled;
    ETeam team;
    BSObject *object;
};
#endif
