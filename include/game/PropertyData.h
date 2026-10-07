#ifndef GAME_PROPERTY_DATA_H
#define GAME_PROPERTY_DATA_H
#include "BPD.h"

// Original GR8E69 conversion/setup symbols; see docs/BPD.md.
void EndianSwap(BPDHeader &);
void EndianSwap(xyzProperty_Struct &);
void EndianSwap(MOH_core_Struct &);
void EndianSwap(MOH_mechanic_Struct &);
void EndianSwap(MOH_mechanicEnvMod_Struct &);
void EndianSwap(MOH_enemy_Struct &);
void EndianSwap(MOH_animatedLight_Struct &);
void EndianSwap(BPDLightVolume &);
void EndianSwapList(unsigned long *);
void PatchUpCore(MOH_core_Struct *);
void PatchUpMechanic(MOH_mechanic_Struct *);
void PatchUpMechEnvMod(MOH_mechanicEnvMod_Struct *);
void PatchUpEnemy(MOH_enemy_Struct *);
void PatchUpAnimLight(MOH_animatedLight_Struct *, int);
int PatchUpAllPropertyData(BPDHeader *);
void FreePropertyMemory();
#endif
