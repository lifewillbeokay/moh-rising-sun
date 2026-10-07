#ifndef GAME_BPD_H
#define GAME_BPD_H

// Scoped GameCube storage views for conversion and pointer setup. Verified array
// strides are documented in docs/BPD.md; other prefixes are not allocation types.
// Opaque prefixes and gaps must not be assigned an inferred historical type.
struct BPDPolyPath;
struct BPDPathFindingNode;
struct BPDPathFindingArea;
struct BPDPathFindingBSP;
struct PropVec3;
struct PropPlane4 { float x, y, z, d; };
struct BPDLight {
    int type;
    float position[3], direction[3], color[3], intensity;
    unsigned char unknown2c[4];
};
struct BPDLightVolume;
struct BPDHeader {
    int field00, field04, field08;
    unsigned char unknown0c[4];
    BPDPolyPath *paths;
    int pathCount;
    BPDPathFindingNode *nodes;
    int nodeCount;
    void **pointerTable;
    int field24;
    void *animatedLights;
    int animatedLightCount;
    void *lightPatterns;
    int lightPatternCount;
    BPDLightVolume *lightVolumes;
    int lightVolumeCount;
    void *classLayouts;
    int classLayoutCount;
    void *objects;
    int objectCount;
    int stringCount;
    void *strings;
    BPDPathFindingArea *areas;
    int areaCount;
    PropVec3 *points;
    int pointCount;
    BPDPathFindingBSP *bsp;
    int bspCount;
};
// GameCube setup establishes 48-byte BPDLight and BPDLightVolume strides.
// Their final four bytes are untouched and retain no inferred type.
struct BPDLightVolume {
    float minimum[3], maximum[3];
    int planeCount;
    PropPlane4 *planes;
    int lightCount;
    BPDLight *lights;
    int priority;
    unsigned char unknown2c[4];
};
struct xyzProperty_Struct {
    int field00;
    unsigned long field04;
    int field08, field0c;
    float field10, field14, field18, field1c, field20, field24, field28;
};
struct MOH_core_Struct {
    unsigned char unknown00[0x2c];
    short field2c, field2e, field30, field32;
    unsigned char unknown34[2];
    short field36, field38, field3a, field3c;
    unsigned char unknown3e[2];
    char *field40;
    unsigned long *field44;
    int field48;
    char *field4c;
    unsigned long field50;
    char *field54;
    unsigned long *field58, *field5c, *field60, *field64, *field68;
    unsigned long *field6c; // Relocated by PatchUpCore; not converted by EndianSwap.
};
struct MOH_mechanic_Struct {
    MOH_core_Struct core;
    unsigned char unknown70[4];
    int field74;
};
struct MOH_enemy_Struct {
    MOH_core_Struct core;
    unsigned char unknown70[14];
    short field7e, field80, field82, field84, field86, field88, field8a, field8c, field8e;
    int field90, field94, field98, field9c, fielda0, fielda4, fielda8, fieldac;
};
struct MOH_mechanicEnvMod_Struct {
    MOH_core_Struct core;
    unsigned char unknown70[4];
    int field74, field78;
};
struct MOH_animatedLight_Struct {
    MOH_core_Struct core;
    unsigned char unknown70[2];
    short field72;
    unsigned long *colours;
    float *times;
};
#endif
