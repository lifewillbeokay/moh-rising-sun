// AI-assisted reconstruction from GR8E69. Fragments of the original bot object source: the
// original file name is not known. Method names come from the symbol table; member names
// are inferred, see docs/Behaviors.md.
#include "AIBot.h"

extern int g_bTeamDM;

void CBotObject::TeamAdjustDistance(float &distance, CVector3 &position) {
    if (g_bTeamDM)
        distance += position.DistanceXY(teamCentre) * character.skills[3];
}
