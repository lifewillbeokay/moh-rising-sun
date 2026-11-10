// CGameSettings: storing the level state and the serialized part of a save.
#include "GameSettingsViews.h"
#include "CameraShake.h"
#include "Fog.h"
#include "Objectives.h"
#include "SoundAmbience.h"
#include "TriggerObject.h"

void CGameSettings::StoreSaveData(unsigned int slot, unsigned int player) {
    slotBackupSlot = slot;
    slotBackupPlayer = player;
    profile->profiles[profile->profileIndex[player]].currentSaveSlot = slot;
    StoreSaveData(true);
    profile->saveData.saveTime = Timestamp::Now();
    slotBackup = profile->profiles[profile->profileIndex[player]].saves[slot];
    slotBackupValid = 1;
    profile->profiles[profile->profileIndex[player]].saves[slot] = profile->saveData;
}
void CGameSettings::StoreSaveData(bool serialize) {
    SoundGetOneShotBankID(profile->saveData.oneShotBank);
    SoundGetAmbientTrackEvent(profile->saveData.ambientTrack);
    SoundGetMusicEventID(profile->saveData.musicEvent);
    if (serialize && profile->saveData.savePointID) {
        serializeMode = 1;
        profile->saveData.buffer.SaveBegin();
        g_Objectives.Serialize();
        SerializeValue(g_NumPlayers, 4);
        for (int i = 0; i < g_NumPlayers; i++) g_scene.GetPlayer(i)->StoreSaveData();
        float start = g_fog.start;
        float end = g_fog.end;
        unsigned int colour = g_fog.colour;
        SerializeValue(start);
        SerializeValue(end);
        SerializeValue(((unsigned char *)&colour)[0]);
        SerializeValue(((unsigned char *)&colour)[1]);
        SerializeValue(((unsigned char *)&colour)[2]);
        SerializeValue(((unsigned char *)&colour)[3]);
        TriggerObject::Save();
        printf("<!!!> The bitstream required %d bytes.\n", profile->saveData.buffer.Used());
        profile->saveData.buffer.SaveEnd();
        serializeMode = 0;
    }
    levelStartData = profile->saveData;
}
