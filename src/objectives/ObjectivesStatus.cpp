// AI-assisted reconstruction from GR8E69; see docs/Objectives.md.
#include "Objectives.h"
void CPlayerObjectives::SetObjectiveStatus(unsigned int id, bool completed) {
    unsigned int index = id - 1;
    if (index > 9)
        return;
    objectives[index].completed = completed;
}
int CPlayerObjectives::GetObjectiveStatus(unsigned int id) {
    unsigned int index = id - 1;
    if (index > 9)
        return 0;
    return objectives[index].completed;
}
int CPlayerObjectives::GetNumCompletedObjectives() {
    int count = 0;
    for (int i = 0; i < 10; i++)
        if (objectives[i].completed && !objectives[i].bonus)
            count++;
    return count;
}
int CPlayerObjectives::CheckIfAllMainObjectivsAreCompleted() { return GetNumCompletedObjectives() == mainCount; }
int CPlayerObjectives::GetNumBonusObjectives() { return bonusCount; }
int CPlayerObjectives::GetNumCompletedBonusObjectives() {
    int count = 0;
    for (int i = 0; i < 10; i++)
        if (objectives[i].completed && objectives[i].bonus)
            count++;
    return count;
}
