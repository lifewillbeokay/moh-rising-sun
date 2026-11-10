// CGameSettings: the settings singleton and its saved data.
#include "GameSettingsViews.h"

CMovieSettings::CMovieSettings() {}
void CMovieSettings::Reset() {
    movieLoad = 0;
    movieMemory = 0;
    introMovie[0] = 0;
    introText[0] = 0;
    introTime[0] = 0;
    outroMovie[0] = 0;
    outroText[0] = 0;
    outroTime[0] = 0;
    creditsMovie[0] = 0;
    creditsText[0] = 0;
    creditsTime[0] = 0;
    movieFont[0] = 0;
    movieBack[0] = 0;
}
void CMovieSettings::Load(CConfigFile &config, int mission, int level) {
    char name[64];
    sprintf(name, "%s", "game");
    LoadSection(config, name);
    sprintf(name, "%d", mission);
    LoadSection(config, name);
    sprintf(name, "%d_%d", mission, level);
    LoadSection(config, name);
}
void CMovieSettings::LoadSection(CConfigFile &config, const char *section) {
    if (config.SetSection(section)) {
        config.AsBool("movie_load", movieLoad);
        config.AsInt("movie_memory", movieMemory);
        config.AsString("intro_movie", introMovie, 64);
        config.AsString("intro_text", introText, 64);
        config.AsString("intro_time", introTime, 64);
        config.AsString("outro_movie", outroMovie, 64);
        config.AsString("outro_text", outroText, 64);
        config.AsString("outro_time", outroTime, 64);
        config.AsString("credits_movie", creditsMovie, 64);
        config.AsString("credits_text", creditsText, 64);
        config.AsString("credits_time", creditsTime, 64);
        config.AsString("historical_movie", historicalMovie, 64);
        config.AsString("historical_text", historicalText, 64);
        config.AsString("historical_time", historicalTime, 64);
        config.AsString("letter_movie", letterMovie, 64);
        config.AsString("letter_text", letterText, 64);
        config.AsString("letter_time", letterTime, 64);
        config.AsString("movie_font", movieFont, 64);
        config.AsString("movie_back", movieBack, 64);
        int mission, level;
        config.AsInt("next_mission", mission);
        config.AsInt("next_level", level);
        g_GameSettings.SetNextMissionAndLevel(mission, level, g_GameSettings.GetCurrentSavePointID());
    }
}
CGameSettings::CGameSettings() : profile((SCoreGameSettings *)MohLoader::GameData()), unknown_a8(1) {
    if ((unsigned int)GameSettingReserve < sizeof(SCoreGameSettings)) {
        char message[512];
        sprintf(message, "\n\nERROR!\n\nThere is not enough memory reserved\nin the loader for a copy of\nSCoreGameSettings.  Please fix.\n\nsizeof( settings reserve  ) = %6d.\nsizeof( SCoreGameSettings ) = %6d.\n", GameSettingReserve, sizeof(SCoreGameSettings));
    }
    serializeMode = 0;
    savePointPending = 0;
    savePointSoft = 0;
}
void CGameSettings::PerformTotalReset(const char *name) {
    PerformTotalReset();
    CopyNameIntoSettings(name, 0);
}
void CGameSettings::PerformTotalReset() {
    profile->TotalReset();
    for (unsigned int i = 0; i <= 3; i++) {
        SetUpButtonConfig(0, profile->profiles[i].buttons[0]);
        SetUpButtonConfig(0, profile->profiles[i].buttons[1]);
    }
    loadedData = 0;
    unknown_a8 = 1;
    unknown_ac = 0;
    nextMission = 1;
    nextLevel = 16;
    CopyNameIntoSettings("DEFAULT", 0);
    for (int p = 0; p <= 3; p++) InitializeFinishedLevelData(p);
    UnlockLevel(1, false);
    UnlockLevel(1, true);
}
void CGameSettings::Init() {
    if (MohLoader::Instance()->gameDataValid != 1) {
        g_GameSettings.profile->language = -1;
        g_GameSettings.PerformTotalReset();
    }
    printf("(FYI) sizeof( SCoreGameSettings ) = %d bytes.  Reserved = %d bytes\n", sizeof(SCoreGameSettings), GameSettingReserve);
    printf("(FYI) sizeof( SPlayerProfile    ) = %d bytes.\n", sizeof(SPlayerProfile));
    LoadData();
    g_GameFlow.levelIsShell = CurrentLevelIsShell();
    {
        CConfigFile config("data\\game.cfg");
        movies.Load(config, g_GameSettings.profile->saveData.mission, g_GameSettings.profile->saveData.level);
        SetSkuType(config);
    }
    if (profile->language == -1) {
        switch (skuType) {
        case 0: Set_Language((ELanguages)0); break;
        case 2: Set_Language((ELanguages)5); break;
        case 3: Set_Language((ELanguages)4); break;
        case 7: Set_Language((ELanguages)12); break;
        case 6: Set_Language((ELanguages)10); break;
        case 5: Set_Language((ELanguages)9); break;
        case 4: Set_Language((ELanguages)2); break;
        case 1:
            profile->profiles[profile->profileIndex[0]].languageSelected = skuType;
            switch (OSGetLanguage()) {
            case 3: Set_Language((ELanguages)2); break;
            case 4: Set_Language((ELanguages)3); break;
            case 5: Set_Language((ELanguages)6); break;
            case 2:
            default:
                Set_Language((ELanguages)1);
                profile->profiles[profile->profileIndex[0]].languageSelected = 0;
                break;
            }
            break;
        default: Set_Language((ELanguages)0); break;
        }
    }
    g_Shell.InitMovies();
    if (loadedData && profile->saveData.levelWon) {
        SCoreGameSettings *core = profile;
        int shell = 0;
        if (core->saveData.mission == 1) shell = core->saveData.level == 16;
        if (!shell) {
            unsigned int bits = core->field_32074;
            int multiplayer = (bits >> 30) & 1;
            int coop = (bits >> 27) & 1;
            int teamDM = (bits >> 24) & 1;
            g_bInMultiplayerMode = multiplayer;
            g_bCoopMultiplayer = coop;
            g_bTeamDM = teamDM;
            if (multiplayer) {
                core->wasLastGameSinglePlayer = shell;
                g_NumPlayers = profile->unknown_3201c[0];
            } else if (coop) {
                core->wasLastGameSinglePlayer = multiplayer;
                g_NumPlayers = 2;
            } else g_NumPlayers = 1;
        }
        printf("Beat level %d\n", profile->profiles[profile->profileIndex[0]].prevLevel);
    }
    ResetCurrentLevelStats();
    ResetLevelTimer();
    profile->saveData.levelWon = 0;
    InitControllerFromOptions(0);
    InvalidateSlotBackup();
}
void CGameSettings::InvalidateSlotBackup() { slotBackupValid = 0; }
void CGameSettings::PerformLevelReset() {
    movies.Reset();
    {
        CConfigFile config("data\\game.cfg");
        movies.Load(config, g_GameSettings.profile->saveData.mission, g_GameSettings.profile->saveData.level);
    }
    if (profile->saveData.savePointID == 0) {
        ResetCurrentLevelStats();
        profile->saveData.levelWon = 0;
        profile->saveData.LevelReset();
        g_GameSettings.SetNextMissionAndLevel(1, 16, 0);
    }
}
void CGameSettings::SetPrevMissionAndLevel(unsigned int mission, unsigned int level) {
    profile->profiles[profile->profileIndex[0]].prevMission = mission;
    if (IsCoop()) {
        profile->profiles[profile->profileIndex[0]].coopPrevLevel = level;
        profile->profiles[profile->profileIndex[1]].coopPrevLevel = level;
    } else profile->profiles[profile->profileIndex[0]].prevLevel = level;
}
void CGameSettings::SetNextMissionAndLevel(unsigned int mission, unsigned int level, int savePoint) {
    nextMission = mission;
    nextLevel = level;
    profile->saveData.savePointID = savePoint;
}
void CGameSettings::SetCurrentMissionAndLevel(unsigned int mission, unsigned int level) {
    profile->saveData.mission = mission;
    profile->saveData.level = level;
}
void CGameSettings::Set_Language(ELanguages language) {
    profile->language = language;
    switch (language) {
    case 0: languageIndex = 0; languageCode = ""; break;
    case 2: languageIndex = 1; languageCode = "s"; break;
    case 3: languageIndex = 2; languageCode = "i"; break;
    case 4: languageIndex = 0; languageCode = ""; break;
    case 5: languageIndex = 3; languageCode = ""; break;
    default: languageIndex = 0; languageCode = ""; break;
    }
    languageCode = "";
}
void CGameSettings::ResetCurrentLevelStats() {
    profile->saveData.stats.Reset();
    profile->levelStats.Reset();
}
void CGameSettings::Set_shotsFired(unsigned int count, bool) {
    profile->saveData.stats.dirty = 1;
    profile->saveData.stats.shotsFired += count;
}
void CGameSettings::Set_hits(unsigned int count, bool) {
    profile->saveData.stats.dirty = 1;
    profile->saveData.stats.hits += count;
}
void CGameSettings::Set_hitsTaken(unsigned int count, bool) {
    profile->saveData.stats.dirty = 1;
    profile->saveData.stats.hitsTaken += count;
}
void CGameSettings::Set_enemiesKilled(unsigned int count, bool) {
    profile->saveData.stats.dirty = 1;
    profile->saveData.stats.enemiesKilled += count;
}
void CGameSettings::Set_shotLocation(unsigned int count, EShotLocations location, bool) {
    profile->saveData.stats.dirty = 1;
    profile->saveData.stats.shotLocation[location] += count;
}
