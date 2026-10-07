// AI-assisted reconstruction from GR8E69; see docs/Objectives.md.
#include "Objectives.h"
void CPlayerObjectives::ShowObjective(unsigned int id) {
    unsigned int index = id - 1;
    if (index > 9)
        return;
    objectives[index].shown = 1;
}
