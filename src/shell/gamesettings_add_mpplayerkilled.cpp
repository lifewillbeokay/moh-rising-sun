// CGameSettings: statistics and level records.
#include "GameSettingsViews.h"
#include "Objectives.h"
#include "Toilet.h"
void iDebugMsg(const char *, ...);
void DebugMsg(const char *, ...);
extern int g_bMPEndLevel;

void CGameSettings::Add_MPPlayerKilled(IMovingSceneNode *killer, IMovingSceneNode *victim) {
    if (g_bMPEndLevel) return;
    int killerTeam = killer->GetTeam();
    int victimTeam = victim->GetTeam();
    int killerIndex = killer->GetPlayerIndex();
    int victimIndex = victim->GetPlayerIndex();
    DebugMsg("Team %d killed Team %d \n", killerTeam, victimTeam);
    if (g_bTeamDM) {
        if (killerTeam == victimTeam) {
            profile->dmStats[killerIndex].teamKills++;
            profile->dmStats[killerIndex].score--;
            profile->teamScore[killerTeam]--;
            profile->dmStats[victimIndex].deaths++;
        } else {
            profile->dmStats[killerIndex].score++;
            profile->teamScore[killerTeam]++;
            profile->dmStats[killerIndex].kills++;
            profile->dmStats[victimIndex].deaths++;
        }
    } else if (killer->GetPlayerIndex() == victim->GetPlayerIndex()) {
        profile->dmStats[killerIndex].score--;
        profile->dmStats[killerIndex].deaths++;
    } else {
        profile->dmStats[killerIndex].score++;
        profile->dmStats[killerIndex].kills++;
        profile->dmStats[victimIndex].deaths++;
    }
    int score;
    if (g_bTeamDM) score = profile->teamScore[killerTeam];
    else score = profile->dmStats[killerIndex].score;
    int limit = profile->players[0].scoreLimit;
    if (limit && limit <= score) g_bMPEndLevel = 1;
}
void CGameSettings::InitializeFinishedLevelData(int player) {
    for (int p = 0; p < 4; p++) {
        memset(profile->profiles[profile->profileIndex[player]].levelData, 0, sizeof(SPlayerLevelData));
        int i;
        SPlayerProfile &record = profile->profiles[profile->profileIndex[player]];
        for (i = 0; i < 11; i++) {
            record.levelData[i].health = 0.0f;
            record.levelData[i].finished = 0;
            record.levelData[i].mission = 1;
            record.levelData[i].level = i + 1;
        }
        for (i = 0; i < 11; i++) {
            record.levelData[i + 11].finished = 0;
            record.levelData[i + 11].mission = 5;
            record.levelData[i + 11].level = i + 1;
            record.levelData[i + 11].health = 0.0f;
        }
    }
}
