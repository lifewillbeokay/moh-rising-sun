// CGameSettings: statistics and level records.
#include "GameSettingsViews.h"
#include "Objectives.h"
#include "Toilet.h"
void iDebugMsg(const char *, ...);
void DebugMsg(const char *, ...);
extern int g_bMPEndLevel;

void CGameSettings::CalculateLevelStats() {
    unsigned int i = 0;
    unsigned int best = 0;
    for (; i <= 7; i++) {
        iDebugMsg("Current hit stats for Weapon[ %d ] = %d \n", i, profile->saveData.stats.entries[i].count);
        if (profile->saveData.stats.entries[i].count > best) best = profile->saveData.stats.entries[i].count;
    }
    best = 0;
    for (i = 0; i < 7; i++) {
        if (profile->saveData.stats.shotLocation[i] > best) best = profile->saveData.stats.shotLocation[i];
    }
}
SLevelStatsSummary CGameSettings::GetLevelStats() const {
    SLevelStatsSummary s;
    memset(&s, 0, sizeof s);
    s.bonusObjectives = g_Objectives.GetNumBonusObjectives() ? g_Objectives.GetNumCompletedBonusObjectives() : 11;
    s.objectives = g_Objectives.GetNumCompletedObjectives();
    s.shotsFired = profile->saveData.stats.shotsFired;
    s.hits = profile->saveData.stats.hits;
    s.enemiesKilled = profile->saveData.stats.enemiesKilled;
    s.hitsTaken = profile->saveData.stats.hitsTaken;
    s.shotLocation[0] = profile->saveData.stats.shotLocation[0];
    s.shotLocation[1] = profile->saveData.stats.shotLocation[3];
    s.shotLocation[2] = profile->saveData.stats.shotLocation[6];
    s.shotLocation[3] = profile->saveData.stats.shotLocation[4];
    s.shotLocation[4] = profile->saveData.stats.shotLocation[1];
    s.shotLocation[5] = profile->saveData.stats.shotLocation[5];
    s.shotLocation[6] = profile->saveData.stats.shotLocation[2];
    return s;
}
SLevelStatsSummary CGameSettings::GetTotalStatsForLevel() const {
    SLevelStatsSummary s;
    memset(&s, 0, sizeof s);
    const SPlayerStats *stats;
    int index = g_Shell.selectedLevel - 1;
    if (IsCoop()) stats = &profile->profiles[profile->profileIndex[0]].coopLevelStats[index];
    else stats = &profile->profiles[profile->profileIndex[0]].levelStats[index];
    s.objectives = stats->objectives;
    s.bonusObjectives = stats->bonusObjectives;
    s.shotsFired = stats->shotsFired;
    s.hits = stats->hits;
    s.enemiesKilled = stats->enemiesKilled;
    s.hitsTaken = stats->hitsTaken;
    s.shotLocation[0] = stats->shotLocation[0];
    s.shotLocation[1] = stats->shotLocation[3];
    s.shotLocation[2] = stats->shotLocation[6];
    s.shotLocation[3] = stats->shotLocation[4];
    s.shotLocation[4] = stats->shotLocation[1];
    s.shotLocation[5] = stats->shotLocation[5];
    s.shotLocation[6] = stats->shotLocation[2];
    return s;
}
void CGameSettings::CalculateCumulativeStats() {
    int mission = profile->saveData.mission;
    int index = profile->saveData.level - 1;
    if ((profile->saveData.stats.dirty && index != 15 && mission == 1) || mission == 5) {
        profile->saveData.stats.dirty = 0;
        SPlayerStats *total;
        if (mission == 5) total = &profile->profiles[profile->profileIndex[0]].coopLevelStats[index];
        else total = &profile->profiles[profile->profileIndex[0]].levelStats[index];
        total->shotsFired += profile->saveData.stats.shotsFired - profile->levelStats.shotsFired;
        total->hits += profile->saveData.stats.hits - profile->levelStats.hits;
        total->enemiesKilled += profile->saveData.stats.enemiesKilled - profile->levelStats.enemiesKilled;
        total->hitsTaken += profile->saveData.stats.hitsTaken - profile->levelStats.hitsTaken;
        if (g_Objectives.GetNumCompletedObjectives() > total->objectives) total->objectives = g_Objectives.GetNumCompletedObjectives();
        if (g_Objectives.GetNumCompletedBonusObjectives() > total->bonusObjectives) total->bonusObjectives = g_Objectives.GetNumCompletedBonusObjectives();
        total->shotLocation[0] += profile->saveData.stats.shotLocation[0] - profile->levelStats.shotLocation[0];
        total->shotLocation[3] += profile->saveData.stats.shotLocation[3] - profile->levelStats.shotLocation[3];
        total->shotLocation[6] += profile->saveData.stats.shotLocation[6] - profile->levelStats.shotLocation[6];
        total->shotLocation[4] += profile->saveData.stats.shotLocation[4] - profile->levelStats.shotLocation[4];
        total->shotLocation[1] += profile->saveData.stats.shotLocation[1] - profile->levelStats.shotLocation[1];
        total->shotLocation[5] += profile->saveData.stats.shotLocation[5] - profile->levelStats.shotLocation[5];
        total->shotLocation[2] += profile->saveData.stats.shotLocation[2] - profile->levelStats.shotLocation[2];
        if (mission == 5) {
            profile->profiles[profile->profileIndex[1]].coopLevelStats[index] = profile->profiles[profile->profileIndex[0]].coopLevelStats[index];
        }
        profile->levelStats = profile->saveData.stats;
    }
}
int CGameSettings::CalculatePerformanceRating(PerformanceStars &accuracy, PerformanceStars &damage, PerformanceStars &bonus) {
    LevelFileContentsStruct_ *tuning = TLT_FindNextResourceByType((TLTResourceID)27, 0);
    {
        int one = 25, two = 35, three = 45;
        if (tuning) {
            if (tuning->data[71]->value) one = tuning->data[71]->value;
            if (tuning->data[72]->value) two = tuning->data[72]->value;
            if (tuning->data[73]->value) three = tuning->data[73]->value;
            if (one < 0) one = 0;
            if (two < 0) two = 0;
            if (three < 0) three = 0;
        }
        int percent;
        if (profile->saveData.stats.shotsFired) percent = profile->saveData.stats.hits * 100 / profile->saveData.stats.shotsFired;
        else percent = 0;
        if (percent >= three) accuracy = ThreeStars;
        else if (percent >= two) accuracy = TwoStars;
        else if (percent >= one) accuracy = OneStar;
        else accuracy = NoStars;
    }
    {
        int completed = g_Objectives.GetNumCompletedBonusObjectives();
        int three = g_Objectives.GetNumBonusObjectives();
        int one = 1, two = 2;
        if (tuning) {
            if (tuning->data[74]->value) one = tuning->data[74]->value;
            if (tuning->data[75]->value) two = tuning->data[75]->value;
            if (tuning->data[76]->value) three = tuning->data[76]->value;
            if (one < 0) one = 0;
            if (two < 0) two = 0;
            if (three < 0) three = 0;
        }
        if (completed >= three) bonus = ThreeStars;
        else if (completed >= two) bonus = TwoStars;
        else if (completed >= one) bonus = OneStar;
        else bonus = NoStars;
    }
    {
        int one = 200, two = 150, three = 100;
        if (tuning) {
            if (tuning->data[77]->value) one = tuning->data[77]->value;
            if (tuning->data[78]->value) two = tuning->data[78]->value;
            if (tuning->data[79]->value) three = tuning->data[79]->value;
            if (one < 0) one = 0;
            if (two < 0) two = 0;
            if (three < 0) three = 0;
        }
        int taken = profile->saveData.stats.hitsTaken;
        if (taken < three) damage = ThreeStars;
        else if (taken < two) damage = TwoStars;
        else if (taken < one) damage = OneStar;
        else damage = NoStars;
    }
    return damage == ThreeStars && bonus == ThreeStars && accuracy == ThreeStars;
}
