// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic reference.
// Part of the original hearingvolume.cpp; see docs/SceneObjects.md.
#include "HearingVolume.h"

EClsnId CHearingVolume::GetCollisionId() const { return collisionId; }

void CHearingVolume::SetCollisionId(EClsnId id) { collisionId = id; }
