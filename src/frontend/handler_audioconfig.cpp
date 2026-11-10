// HandlerAudioConfig, the audio settings screen; see docs/Frontend.md.
#include "PackageHandlers.h"
#include "GameSettings.h"
#include "GameSettingsViews.h"
#include "PauseMenu.h"
#include <stdio.h>
void SoundGetVolumes(float &, float &);
int HandlerAudioConfig::HandleEvent(unsigned int event, FEObject *, unsigned long) {
    int direction = 0;
    if (event == FEHashUpper("Pad_Left")) {
        direction = -1;
    } else if (event == FEHashUpper("Pad_Right")) {
        direction = 1;
    } else if (event == FEHashUpper("Audio Config Setup")) {
        EnableButton(package, "Sound Format Button");
        int player = 0;
        SetScript(package, "Sound Format text", "Show");
        if (g_Shell.inCoop || (g_GameFlow.levelIsShell && !g_GameSettings.WasLastGameSinglePlayer())) {
            player = g_Shell.coopPausePlayer;
            SetProfileNameOnScreen(player);
        } else {
            SetProfileNameOnScreen(0);
        }
        if (g_PauseScreen.paused) player = 0;
        if (!confirmed) {
            Revert();
        } else {
            soundFormat = g_GameSettings.GetSoundModeValue(player);
            subtitles = g_GameSettings.profile->profiles[g_GameSettings.profile->profileIndex[player]].languageSelected;
            SoundGetVolumes(savedSfxVolume, savedMusicVolume);
            SoundSetVolumes(g_GameSettings.profile->profiles[g_GameSettings.profile->profileIndex[player]].volumeA,
                            g_GameSettings.profile->profiles[g_GameSettings.profile->profileIndex[player]].volumeB);
            knobs.Setup(VolumeKnobMusic, package->FindObjectByName("music volume knob"));
            knobs.Setup(VolumeKnobSfx, package->FindObjectByName("sfx volume knob"));
        }
    } else if (event == FEHashUpper("Audio Config Confirm")) {
        int player = 0;
        if (g_Shell.inCoop || (g_GameFlow.levelIsShell && !g_GameSettings.WasLastGameSinglePlayer()))
            player = g_Shell.coopPausePlayer;
        if (g_PauseScreen.paused) player = 0;
        g_Shell.PlayShellSound((EShellSoundType)11);
        bool changed = false;
        if (soundFormat != g_GameSettings.GetSoundModeValue(player)) {
            g_GameSettings.SetSoundModeValue(player, (ESoundMode)soundFormat);
            changed = true;
        }
        g_GameSettings.profile->profiles[g_GameSettings.profile->profileIndex[player]].languageSelected = subtitles;
        float sfx, music;
        SoundGetVolumes(sfx, music);
        g_GameSettings.profile->profiles[g_GameSettings.profile->profileIndex[player]].volumeA = sfx;
        g_GameSettings.profile->profiles[g_GameSettings.profile->profileIndex[player]].volumeB = music;
        g_GameSettings.InitAudioFromOptions(player, changed);
        confirmed = 1;
    } else if (event == FEHashUpper("Audio Config Revert")) {
        Revert();
    } else {
        printf("Unknown event %x from audio config screen\n", event);
        return 0;
    }
    if (direction) {
        confirmed = 0;
        FEObject *button = package->currentButton;
        if (button->hash == FEHashUpper("SFX Volume button")) {
            if (direction > 0) knobs.Increase(VolumeKnobSfx);
            else knobs.Decrease(VolumeKnobSfx);
        } else if (button->hash == FEHashUpper("Music Volume button")) {
            if (direction > 0) knobs.Increase(VolumeKnobMusic);
            else knobs.Decrease(VolumeKnobMusic);
        } else if (button->hash == FEHashUpper("Sound Format button")) {
            g_Shell.PlayShellSound((EShellSoundType)1);
            soundFormat += direction;
            if (soundFormat < 0) soundFormat = 2;
            if (soundFormat > 2) soundFormat = 0;
        } else if (button->hash == FEHashUpper("Subtitles button")) {
            subtitles ^= 1;
            if (subtitles) g_Shell.PlayShellSound((EShellSoundType)1);
            else g_Shell.PlayShellSound((EShellSoundType)0);
        } else {
            printf("Unknown audio option '%s'!\n", button->name);
        }
    }
    switch (soundFormat) {
    case 1:
        SetScript(package, "Stereo", "Show");
        SetScript(package, "Dolby", "Hide");
        SetScript(package, "Mono", "Hide");
        break;
    case 2:
        SetScript(package, "Stereo", "Hide");
        SetScript(package, "Dolby", "Show");
        SetScript(package, "Mono", "Hide");
        break;
    case 0:
        SetScript(package, "Stereo", "Hide");
        SetScript(package, "Dolby", "Hide");
        SetScript(package, "Mono", "Show");
        break;
    default:
        printf("Unknown audio format value %d!\n", soundFormat);
        break;
    }
    if (subtitles) {
        SetScript(package, "Subtitles On", "Show");
        SetScript(package, "Subtitles Off", "Hide");
    } else {
        SetScript(package, "Subtitles On", "Hide");
        SetScript(package, "Subtitles Off", "Show");
    }
    return 0;
}
void HandlerAudioConfig::Revert() {
    int player = 0;
    if (g_Shell.inCoop || (g_GameFlow.levelIsShell && !g_GameSettings.WasLastGameSinglePlayer()))
        player = g_Shell.coopPausePlayer;
    SoundSetVolumes(savedSfxVolume, savedMusicVolume);
    soundFormat = g_GameSettings.GetSoundModeValue(player);
    subtitles = g_GameSettings.profile->profiles[g_GameSettings.profile->profileIndex[player]].languageSelected;
    SoundGetVolumes(savedSfxVolume, savedMusicVolume);
    knobs.Setup(VolumeKnobMusic, package->FindObjectByName("music volume knob"));
    knobs.Setup(VolumeKnobSfx, package->FindObjectByName("sfx volume knob"));
}
