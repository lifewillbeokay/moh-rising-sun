// Scoped declarations shared by the CGameSettings source fragments; see docs/Frontend.md.
#ifndef GAME_GAME_SETTINGS_VIEWS_H
#define GAME_GAME_SETTINGS_VIEWS_H
#include "ConfigFile.h"
#include "CQuaternion.h"

#include "GameSettings.h"
#include "MohLoader.h"
#include "SceneNode.h"
#include "ShellManager.h"
#include <stdio.h>
#include <string.h>

extern CScene g_scene;
extern int g_bCoopMultiplayer;
extern int g_bInMultiplayerMode;
void SoundSetVolumes(float, float);
void AmbientTrack_Select(int);
void MUSIC_SendEvent(int);
bool APlayerStartExistsForSavePoint(int);
extern int g_bTeamDM;
extern int g_NumPlayers;
extern "C" unsigned int OSGetSoundMode(void);
extern "C" void OSSetSoundMode(unsigned int);
extern int g_uSavePointID[32];
extern int g_bSavePointSoft[32];
struct KEYMAP { unsigned short key; unsigned short event; };
class CPlayerHudView { public: unsigned char unknown_0[0x1c18]; int hudOption; };
class CPlayerTimerView { public: unsigned char unknown_0[0xd4c]; unsigned int unknown_top : 1; unsigned int timerPaused : 1; };
class CSaveGameKeyboardPopup { public: static CSaveGameKeyboardPopup *Instance(); void CreateName(); };
#include "PauseMenu.h"
// Absolute linker symbol: the loader's reservation for SCoreGameSettings.
extern char GameSettingReserve[];
extern "C" unsigned char OSGetLanguage(void);
#define IsCoop() (g_Shell.inCoop || g_bCoopMultiplayer)

#endif
