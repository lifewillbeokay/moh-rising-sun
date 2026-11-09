// Scoped property-reader views adapted from the CC0 Frontline reconstruction.
// GR8E69 evidence and provenance: docs/research/broad-source-sweep.md.
#ifndef GAME_PROPERTY_READERS_H
#define GAME_PROPERTY_READERS_H
#include "BPD.h"
struct MOH_fogParams_Struct {
    MOH_core_Struct core;
    short field70;
    short field72;
    short field74;
    unsigned char unknown76[2];
    int field78;
    int field7c;
};
struct MOH_playerPathController_Struct {
    MOH_core_Struct core;
    unsigned char field70;
    unsigned char field71;
    short field72;
    short field74;
    unsigned char unknown76[2];
    int field78;
    int field7c;
};
struct MOH_cameraShaker_Struct {
    MOH_core_Struct core;
    unsigned char field70;
    unsigned char field71;
    unsigned char unknown72[2];
    int field74;
    int field78;
    int field7c;
    int field80;
    int field84;
};
struct MOH_projectileGeneratorTarget_Struct {
    MOH_core_Struct core;
    unsigned char field70;
    unsigned char field71;
    short field72;
    int field74;
    int field78;
    int field7c;
    int field80;
    int field84;
    int field88;
};
struct MOH_projectileGenerator_Struct {
    MOH_core_Struct core;
    unsigned char field70;
    unsigned char field71;
    short field72;
    int field74;
    int field78;
    int field7c;
    int field80;
    int field84;
    int field88;
    int field8c;
    int field90;
    int field94;
};
struct MOH_envMod_Struct {
    MOH_core_Struct core;
    int field70;
};
struct MOH_pathPoint_Struct {
    MOH_core_Struct core;
};
struct MOH_wayPoint_Struct {
    MOH_core_Struct core;
    union {
        struct {
            unsigned int flag22 : 1, flag23 : 1, flag24 : 1, flag25 : 1, flag26 : 1, flag27 : 1, flag28 : 1,
                unknownFlags : 25;
        };
        unsigned char field70;
    };
};
struct MOH_powerup_Struct {
    MOH_core_Struct core;
    unsigned char field70;
};
int _LoadPropertyData(MOH_fogParams_Struct *, int);
int _LoadPropertyData(MOH_animatedLight_Struct *, int);
int _LoadPropertyData(MOH_playerPathController_Struct *, int);
int _LoadPropertyData(MOH_cameraShaker_Struct *, int);
int _LoadPropertyData(MOH_projectileGeneratorTarget_Struct *, int);
int _LoadPropertyData(MOH_projectileGenerator_Struct *, int);
int _LoadPropertyData(MOH_mechanicEnvMod_Struct *, int);
int _LoadPropertyData(MOH_envMod_Struct *, int);
int _LoadPropertyData(MOH_pathPoint_Struct *, int);
int _LoadPropertyData(MOH_wayPoint_Struct *, int);
int _LoadPropertyData(MOH_powerup_Struct *, int);
int _LoadPropertyData(MOH_enemy_Struct *, int);
int _LoadPropertyData(MOH_mechanic_Struct *, int);
int _LoadPropertyData(MOH_core_Struct *, int);
int LoadPropertyData(xyzProperty_Struct *, int);
#endif
