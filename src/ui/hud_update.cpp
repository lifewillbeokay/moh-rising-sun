// AI-assisted reconstruction from GR8E69. Part of the original HeadsUpDisplay.cpp. CHeadsUpDisplay::Update
// brings the panels back when ammunition, weapon or health change, and each panel then fades on its own
// timer. There is no Frontline counterpart for this run, so member names are inferred.
#include "HeadsUpDisplay.h"
#include "MathFun.h"
#include "AITargeting.h"
#include "BSGameObject.h"
#include "ScriptRuntime.h"
extern "C" float cosf(float);

// Method-only view of the player for the HUD's counters.
class CPlayerObject {
  public:
    int GetCurrentWeaponReserveAmoCount() const;
    int GetCurrentWeaponClipAmoCount() const;
    CWeapon *GetCurrentWeapon() const;
    int GetNumGrenades() const;
};

// Method-only view of the input manager; the arguments' meaning is not established.
class CInputManager {
  public:
    void SetFeedback(unsigned char, unsigned char, unsigned char, float, float);
};
extern CInputManager g_inputMgr;

struct PauseScreenFlags {
    unsigned char unknown_0[0x10];
    int paused;     // +0x10
    unsigned char unknown_14[4];
    int inMenu;     // +0x18; name inferred.
};
extern PauseScreenFlags g_PauseScreen;

void CHeadsUpDisplay::UpdateCompass(float elapsed) {
    if (g_PauseScreen.paused || g_PauseScreen.inMenu)
        return;
    float step = elapsed * 0.0166833512f;
    if (compassState == 0) {
        if (unknown808 == 0) {
            float fade = compassTime = MathFunClamp(compassTime - step, 0.0f, 4.5f);
            if (fade > 1.0f)
                fade = 1.0f;
            unknown7d0 = fade * unknown7c8;
        } else {
            unknown7d0 = unknown7c8;
        }
    } else if (compassState == 1) {
        float time = compassTime - step;
        compassTime = time;
        if (!(time > 0.0f)) {
            compassTime = 0.0f;
            unknown7d0 = unknown7c8;
        } else {
            unknown7d0 = unknown7c8 * (cosf(pulsePhase) * 0.5f + 0.5f);
        }
        float rumble = unknown80c - step;
        unknown80c = rumble;
        if (!(rumble > 0.0f)) {
            g_inputMgr.SetFeedback(player->GetPlayerIndex(), 9, 0, 0.0125f, 0.25f);
            unknown80c = 1.0f;
        }
    }
}

void CHeadsUpDisplay::UpdateWeaponName(float elapsed) {
    float fade = unknown7ec = MathFunClamp(unknown7ec - elapsed * 0.0166833512f, 0.0f, 4.5f);
    if (fade > 1.0f)
        fade = 1.0f;
    unknown7d8 = fade * unknown7c8;
}

void CHeadsUpDisplay::UpdateAmmo(float elapsed) {
    float step = elapsed * 0.0166833512f;
    if (ammoState == 0) {
        if (unknown808 == 0) {
            float fade = ammoTime = MathFunClamp(ammoTime - step, 0.0f, 4.5f);
            if (fade > 1.0f)
                fade = 1.0f;
            unknown7d4 = fade * unknown7c8;
        } else {
            unknown7d4 = unknown7c8;
        }
    } else if (ammoState == 1) {
        ammoTime = 1.0f;
        unknown7d4 = unknown7c8 * (cosf(pulsePhase) * 0.5f + 0.5f);
    }
}

void CHeadsUpDisplay::Update(float elapsed) {
    pulsePhase += elapsed * 0.104824588f;
    if (pulsePhase > 6.2831855f)
        pulsePhase -= 6.2831855f;
    if (unknown7cc > 0.0f)
        unknown7cc -= elapsed * 0.0166833512f;
    if (unknown7cc >= 1.0f)
        unknown7c8 = 0.0f;
    else if (unknown7cc > 0.0f)
        unknown7c8 = 1.0f - unknown7cc;
    else
        unknown7c8 = 1.0f;
    if (unknown808)
        ammoTime = compassTime = 1.0f;

    float health = 0.0f;
    BSObject *script = (BSObject *)player->GetScriptObject();
    float *data = 0;
    if (script)
        data = (float *)script->gameObject->GetScriptData();
    if (data)
        health = *data;

    int reserve = ((CPlayerObject *)player)->GetCurrentWeaponReserveAmoCount();
    int clip = ((CPlayerObject *)player)->GetCurrentWeaponClipAmoCount();
    CWeapon *weapon = ((CPlayerObject *)player)->GetCurrentWeapon();
    int grenades = ((CPlayerObject *)player)->GetNumGrenades();
    if (lastWeapon && weapon != lastWeapon)
        unknown7ec = ammoTime = 4.5f;
    if (lastReserveAmmo || lastClipAmmo || lastGrenades) {
        if (reserve != lastReserveAmmo || clip != lastClipAmmo || grenades != lastGrenades)
            ammoTime = 4.5f;
    }
    if (lastHealth != 0.0f && health != lastHealth)
        compassTime = 4.5f;

    UpdateCompass(elapsed);
    UpdateCompassRotation((CPlayerObject *)player, elapsed);
    UpdateAmmo(elapsed);
    UpdateHitStatus(elapsed);
    UpdateWeaponName(elapsed);

    lastHealth = health;
    lastReserveAmmo = ((CPlayerObject *)player)->GetCurrentWeaponReserveAmoCount();
    lastClipAmmo = ((CPlayerObject *)player)->GetCurrentWeaponClipAmoCount();
    lastWeapon = ((CPlayerObject *)player)->GetCurrentWeapon();
    lastGrenades = grenades;

    if (letterboxState) {
        switch (letterboxStyle) {
        case 1:
            break;
        case 2:
            letterboxState = 0;
            break;
        case 3:
            letterboxPosition += letterboxRate * elapsed;
            if (letterboxPosition >= 1.0f)
                letterboxPosition = 1.0f;
            break;
        case 4:
            letterboxPosition += letterboxRate * elapsed;
            if (letterboxPosition <= 0.0f)
                letterboxPosition = 0.0f;
            break;
        }
    }
}

void CHeadsUpDisplay::LetterboxTransition(float time, _LetterboxTransitionStyle style) {
    letterboxState = 0;
    letterboxPosition = 1.0f;
    letterboxRate = 0.0f;
    letterboxStyle = style;
    if (time == 0.0)
        time = 1.0f;
    switch (style) {
    case 0:
    default:
        letterboxState = 0;
        break;
    case 2:
        letterboxState = 0;
        break;
    case 1:
        letterboxState = style;
        break;
    case 3:
        letterboxState = 1;
        letterboxPosition = 0.0f;
        letterboxRate = 1.0f / time;
        break;
    case 4:
        letterboxState = 1;
        letterboxPosition = 1.0f;
        letterboxRate = -(1.0f / time);
        break;
    }
}
