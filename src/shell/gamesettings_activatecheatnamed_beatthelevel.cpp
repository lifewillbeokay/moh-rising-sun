// CGameSettings: statistics and level records.
#include "GameSettingsViews.h"
#include "Objectives.h"
#include "Toilet.h"
void iDebugMsg(const char *, ...);
void DebugMsg(const char *, ...);
extern int g_bMPEndLevel;

void CGameSettings::ActivateCheatNamed_BeatTheLevel() {
    unsigned char *prevLevel;
    if (IsCoop()) prevLevel = &profile->profiles[profile->profileIndex[0]].coopPrevLevel;
    else prevLevel = &profile->profiles[profile->profileIndex[0]].prevLevel;
    int level = *prevLevel;
    int next = level + 1;
    if (level == 1) {
        UnlockLevel(11, false);
        profile->profiles[profile->profileIndex[0]].filmCanisters[0] |= 2;
        profile->profiles[profile->profileIndex[0]].filmCanisters[1] |= 2;
        AddMedal(1);
        SetNextMissionAndLevel(1, 11, 0);
        *prevLevel = nextLevel;
    } else if (level == 11) {
        UnlockLevel(2, false);
        AddMedal(11);
        SetNextMissionAndLevel(1, 2, 0);
        *prevLevel = nextLevel;
    } else if (level == 9) {
        profile->profiles[profile->profileIndex[0]].filmCanisters[0] |= 512;
        profile->profiles[profile->profileIndex[0]].filmCanisters[1] |= 512;
        AddMedal(9);
        SetNextMissionAndLevel(1, 16, 0);
        *prevLevel = nextLevel;
    } else if (level == 5) {
        UnlockLevel(7, false);
        profile->profiles[profile->profileIndex[0]].filmCanisters[0] |= 32;
        profile->profiles[profile->profileIndex[0]].filmCanisters[1] |= 32;
        AddMedal(5);
        SetNextMissionAndLevel(1, 7, 0);
        *prevLevel = nextLevel;
    } else if (level <= 8) {
        UnlockLevel(next, false);
        int mask = 1 << level;
        profile->profiles[profile->profileIndex[0]].filmCanisters[0] |= mask;
        profile->profiles[profile->profileIndex[0]].filmCanisters[1] |= mask;
        AddMedal(level);
        SetNextMissionAndLevel(1, next, 0);
        *prevLevel = nextLevel;
    }
    if (IsCoop()) profile->profiles[profile->profileIndex[1]].coopPrevLevel = *prevLevel;
}
void CGameSettings::DoBeatTheLevelDuties(int level) {
    unsigned char *prevLevel;
    if (IsCoop()) prevLevel = &profile->profiles[profile->profileIndex[0]].coopPrevLevel;
    else prevLevel = &profile->profiles[profile->profileIndex[0]].prevLevel;
    if (level == 1) {
        UnlockLevel(11, false);
        SetNextMissionAndLevel(1, 11, 0);
        *prevLevel = nextLevel;
    } else if (level == 11) {
        UnlockLevel(2, false);
        SetNextMissionAndLevel(1, 2, 0);
        *prevLevel = nextLevel;
    } else if (level == 9) {
        SetNextMissionAndLevel(1, 16, 0);
        *prevLevel = nextLevel;
    } else if (level == 5) {
        UnlockLevel(7, false);
        SetNextMissionAndLevel(1, 7, 0);
        *prevLevel = nextLevel;
    } else if (level <= 10) {
        UnlockLevel(level + 1, false);
        SetNextMissionAndLevel(1, level + 1, 0);
        *prevLevel = nextLevel;
    }
    if (IsCoop()) profile->profiles[profile->profileIndex[1]].coopPrevLevel = *prevLevel;
    PerformanceStars accuracy, objectives, damage;
    if (CalculatePerformanceRating(accuracy, objectives, damage)) AddMedal(level);
}
