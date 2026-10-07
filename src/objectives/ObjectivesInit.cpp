// AI-assisted reconstruction from GR8E69; see docs/Objectives.md.
#include "Objectives.h"
CPlayerObjectives::CPlayerObjectives() { Reset(); }
void CPlayerObjectives::Reset() {
    mainCount = 0;
    bonusCount = 0;
    field_d0 = 1;
    for (int i = 0; i < 10; i++) {
        objectives[i].shown = 0;
        objectives[i].completed = 0;
        objectives[i].bonus = 0;
        objectives[i].prompt = 0;
    }
}
