// HandlerControllerOptions, the controller settings screen; see docs/Frontend.md.
#include "PackageHandlers.h"
#include "GameSettings.h"
#include "PauseMenu.h"
#include "MathFun.h"
#include <stdio.h>
int HandlerControllerOptions::HandleEvent(unsigned int event, FEObject *, unsigned long) {
    int direction = 0;
    if (event == FEHashUpper("Pad_Left")) {
        direction = -1;
    } else if (event == FEHashUpper("Pad_Right")) {
        direction = 1;
    } else if (event == FEHashUpper("Controller Setup")) {
        player = direction;
        if (g_Shell.inMultiplayer) {
            player = g_Shell.coopPausePlayer;
            SetScript(package, "Controller Options Choreo", "MPMode");
        } else if (g_Shell.inCoop || (g_GameFlow.levelIsShell && !g_GameSettings.WasLastGameSinglePlayer())) {
            player = g_Shell.coopPausePlayer;
            SetScript(package, "Controller Options Choreo", "SPMode");
        } else {
            SetScript(package, "Controller Options Choreo", "SPMode");
        }
        int settingsPlayer;
        if (g_PauseScreen.paused) settingsPlayer = (signed char)g_PauseScreen.controllingPlayer;
        else settingsPlayer = player;
        int language = GetCurrentLanguage();
        FEWideString *text = g_Shell.localizer->GetString("VIBRATE_GC", language);
        SetObjectString(package, "Vibrate text", text);
        SetObjectString(package, "Profile Name text", g_GameSettings.GetNameFromSettings(player));
        SControllerPrefs prefs = g_GameSettings.profile->profiles[g_GameSettings.GetProfileIndexFromPlayerID(settingsPlayer)].controllerPrefs;
        vibrate = prefs.vibrate != 0;
        crosshairs = prefs.crosshairs != 0;
        turnSpeed = prefs.turnSpeed;
        analogSensitivity = prefs.analogSensitivity;
        invertY = prefs.invertY != 0;
        turnSpeedSlider.Initialize(package->FindObjectByName("Turn Speed knob"));
        turnSpeedSlider.SetValue(turnSpeed);
        sensitivitySlider.Initialize(package->FindObjectByName("Analog sensitivity knob"));
        sensitivitySlider.SetValue(analogSensitivity);
    } else if (event == FEHashUpper("Controller Options Confirm")) {
        g_Shell.PlayShellSound((EShellSoundType)11);
        SControllerPrefs prefs;
        prefs.vibrate = vibrate;
        prefs.crosshairs = crosshairs;
        prefs.invertY = invertY;
        prefs.analogSensitivity = analogSensitivity;
        prefs.turnSpeed = turnSpeed;
        int settingsPlayer;
        if (g_PauseScreen.paused) settingsPlayer = (signed char)g_PauseScreen.controllingPlayer;
        else settingsPlayer = player;
        g_GameSettings.profile->profiles[g_GameSettings.GetProfileIndexFromPlayerID(settingsPlayer)].controllerPrefs = prefs;
        g_GameSettings.InitControllerFromOptions(settingsPlayer);
        printf("Saving controller options\n");
    } else {
        printf("Unknown event from controller screen\n");
        return 0;
    }
    if (direction) {
        FEObject *button = package->currentButton;
        if (button->hash == FEHashUpper("Crosshairs button")) {
            crosshairs ^= 1;
            if (crosshairs) g_Shell.PlayShellSound((EShellSoundType)1);
            else g_Shell.PlayShellSound((EShellSoundType)0);
        } else if (button->hash == FEHashUpper("Vibrate button")) {
            vibrate ^= 1;
            if (vibrate) g_Shell.PlayShellSound((EShellSoundType)1);
            else g_Shell.PlayShellSound((EShellSoundType)0);
        } else if (button->hash == FEHashUpper("Grenade Cook button")) {
            grenadeCook ^= 1;
            if (grenadeCook) g_Shell.PlayShellSound((EShellSoundType)1);
            else g_Shell.PlayShellSound((EShellSoundType)0);
        } else if (button->hash == FEHashUpper("Invert Y Axis button")) {
            invertY ^= 1;
            if (invertY) g_Shell.PlayShellSound((EShellSoundType)1);
            else g_Shell.PlayShellSound((EShellSoundType)0);
        } else if (button->hash == FEHashUpper("Analog Sensitivity button")) {
            analogSensitivity += (float)direction * 0.1f;
            if (!(analogSensitivity < 0.0f) && !(analogSensitivity > 1.0f)) {
                if (direction > 0) g_Shell.PlayShellSound((EShellSoundType)6);
                else g_Shell.PlayShellSound((EShellSoundType)7);
            }
            analogSensitivity = MathFunClamp(analogSensitivity, 0.0f, 1.0f);
            sensitivitySlider.SetValue(analogSensitivity);
        } else if (button->hash == FEHashUpper("Turn Speed button")) {
            turnSpeed += (float)direction * 0.1f;
            if (!(turnSpeed < 0.0f) && !(turnSpeed > 1.0f)) {
                if (direction > 0) g_Shell.PlayShellSound((EShellSoundType)6);
                else g_Shell.PlayShellSound((EShellSoundType)7);
            }
            turnSpeed = MathFunClamp(turnSpeed, 0.0f, 1.0f);
            turnSpeedSlider.SetValue(turnSpeed);
        } else {
            printf("Unknown controller option '%s'!\n", button->name);
        }
    }
    if (crosshairs) {
        SetScript(package, "Crosshairs On", "Show");
        SetScript(package, "Crosshairs Off", "Hide");
    } else {
        SetScript(package, "Crosshairs On", "Hide");
        SetScript(package, "Crosshairs Off", "Show");
    }
    if (vibrate) {
        SetScript(package, "Vibrate On", "Show");
        SetScript(package, "Vibrate Off", "Hide");
    } else {
        SetScript(package, "Vibrate On", "Hide");
        SetScript(package, "Vibrate Off", "Show");
    }
    if (invertY) {
        SetScript(package, "Invert Y Axis On", "Show");
        SetScript(package, "Invert Y Axis Off", "Hide");
    } else {
        SetScript(package, "Invert Y Axis On", "Hide");
        SetScript(package, "Invert Y Axis Off", "Show");
    }
    return 0;
}
