// AI-assisted scoped reconstruction from GR8E69; see docs/CameraShake.md and docs/Paths.md.
#ifndef GAME_CAMERA_SHAKE_H
#define GAME_CAMERA_SHAKE_H
#pragma interface
class CAISplinePath;
class CWeapon;
class CVector3;
class CStaticObject;
class ISceneNode;
struct BSObject;

// Scoped member view of CPlayerObject for shake and path controls. This does not
// establish the complete allocation, base classes or virtual interface.
// Observed fields of the player's embedded clip-plane settings at +0x1150.
struct ClipSettings {
    unsigned char unknown_0[0x38];
    float farClip;      // +0x38
    unsigned char unknown_3c[0x158 - 0x3c];
    int field_158;      // Cleared with field_15c by BIFunc_ChangeClipPlane.
    int field_15c;
    int changed;        // +0x160; set to 1 by BIFunc_ChangeClipPlane.
};

// Original enum tag; its members are unknown.
enum _LetterboxTransitionStyle { LetterboxTransitionStyleUnknown = 0 };

// Method-only view of the player's embedded heads-up display.
class CHeadsUpDisplay {
  public:
    unsigned char unknown_0[0x7cc];
    float field_7cc; // Reset from 0 to 1 by BIFunc_DetachPlayerFromProp.
    void AddHitDirection(unsigned int);
    void LetterboxTransition(float, _LetterboxTransitionStyle);
};

extern "C" void *DWI_alloc(const char *, int, int);
// Member view of the 36-byte sprite the binocular overlay allocates.
class CSprite {
  public:
    enum ESpriteLoadType { SpriteLoadUnknown0 = 0, SpriteLoadUnknown1 = 1 };
    static void *operator new(unsigned int size, const char *label) {
        return DWI_alloc(label, size, 1024);
    }
    CSprite(const char *, ESpriteLoadType);
    unsigned char unknown_0[0x1c];
    int field_1c; // Cleared by BIFunc_UseBinoculars.
    int field_20; // Cleared by BIFunc_UseBinoculars.
};
// Method-only view of the multiplayer player body (CPlayerObject +0x1144).
class CAnimatedPlayer {
  public:
    void GetFlocoParams(float &, int &);
};
class CPlayerWeaponObject {
  public:
    unsigned char unknown_0[0x2ec];
    CWeapon *weapon; // +0x2ec
    unsigned char unknown_2f0[0x3b0 - 0x2f0];
    int state;       // +0x3b0; set to 3 by BIFunc_AddPlayerAmmo.
    unsigned char unknown_3b4[0x3d2 - 0x3b4];
    char name[0x3f0 - 0x3d2]; // +0x3d2; animation folder name (extent unknown).
    unsigned int flags; // +0x3f0; bit 29 set by BIFunc_AddPlayerAmmo before DoneReloading.
    // Original nested enum tag; member names are descriptive.
    enum Partition { PartitionDefault = 0, PartitionAlternate = 1 };
    void CreateEjectedShell(int);
    void DoneReloading();
    void SetAnimationState(const char *, int, Partition);
    int IsSelectable() const;
};
class CPlayerObject {
  public:
    // Member-only view of the embedded look-at controller at +0x1cf0.
    class LookAtTarget {
      public:
        void Enable(ISceneNode *, float);
        void Enable(const CVector3 &, float);
        void Disable();
    };
    // 36 bytes: eight floats, then the table pointer at +32 (the class declares its
    // data before its virtual destructor). Field names are descriptive.
    class CCameraShake {
      public:
        float current;     // Intensity now.
        float target;      // Twice the requested intensity.
        float scale;       // 1 / intensity at the last SetShake (1 when none).
        float field_0c;
        float fadeTime;    // Ramp time used when the duration expires.
        float duration;    // 100000 means no automatic stop.
        float elapsed;
        float rate;        // Intensity change per update step; 0 when settled.
        CCameraShake();
        virtual ~CCameraShake();
        void SetShake(float, float, float, float);
        void SetShake(float, float);
        void Evaluate(float &, float &);
        void Update(float);
    };

    // Member-only view; unknown bytes retain their original ownership.
    struct Flags {
        unsigned int unknown_31_27 : 5;
        unsigned int canCrouch : 1;     // Bit 26: BIFunc_PlayerCanCrouch.
        unsigned int motionShake : 1;   // Bit 25: set by DoMotionShake for a positive amount.
        unsigned int unknown_24 : 1;
        unsigned int wielding : 1;      // Bit 23: BIFunc_SetPlayerWielding.
        unsigned int triggerChecks : 1; // Bit 22: BIFunc_TogglePlayerVsTriggerChecks.
        unsigned int superAimMode : 1;  // Bit 21: BIFunc_PlayerSuperAimMode.
        unsigned int unknown_20_12 : 9;
        unsigned int invincible : 1;    // Bit 11: BIFunc_MakePlayerInvincible.
        unsigned int unknown_low : 11;
    };
    struct PathFlagBits {
        unsigned int unknown_31_25 : 7;
        unsigned int binoculars : 1;    // Bit 24: BIFunc_UseBinoculars.
        unsigned int unknown_23_16 : 8;
        unsigned int meleeImmune : 1;   // Bit 15: BIFunc_CanMeleeTarget returns 0.
        unsigned int noShells : 1;      // Bit 14: suppresses BIFunc_PlayerEjectShell.
        unsigned int unknown_13_10 : 4;
        unsigned int mmgBlocked : 1;    // Bit 9: skips BIFunc_AttachPlayerToMountedMachineGun in multiplayer.
        unsigned int unknown_8_0 : 9;
    };
    unsigned char unknown_0[0xd44];
    union {
        unsigned int pathFlags; // +0xd44; bit 19 enables path movement, bit 27 is set while aiming.
        PathFlagBits pathBits;
    };
    unsigned char unknown_d48[4];
    Flags flags;
    unsigned char unknown_d50[0xd58 - 0xd50];
    CCameraShake cameraShake;
    CCameraShake backgroundShake;
    float motionShakeAmount, motionShakeRate, motionShakeTime;

    unsigned char unknown_dac[0xe00 - 0xdac];
    CAISplinePath *movePath, *lookPath; // +0xe00, +0xe04
    float pathParameter, pathRate; // +0xe08, +0xe0c
    unsigned char unknown_e10[0xe30 - 0xe10];
    float fov;               // +0xe30; set by BIFunc_SetGameFOV.
    unsigned char unknown_e34[0xe40 - 0xe34];
    int fovMode;             // +0xe40
    unsigned char unknown_e44[0xe48 - 0xe44];
    CSprite *binocularOverlay; // +0xe48; set by BIFunc_UseBinoculars.
    unsigned char unknown_e4c[0xe78 - 0xe4c];
    CWeapon *thrownWeapon; // +0xe78; checked by BIFunc_PlayerWeaponInfo when no weapon object matches.
    unsigned char unknown_e7c[0xe80 - 0xe7c];
    CStaticObject *mmgBase; // +0xe80; set by BIFunc_AttachHierBaseToMMG.
    unsigned char unknown_e84[0x10bc - 0xe84];
    int cameraPitchUnhinged; // +0x10bc
    unsigned char unknown_10c0[0x10da - 0x10c0];
    short promptSlot;        // +0x10da; prompt buffer used by BIFunc_AddPlayerAmmo.
    unsigned char unknown_10dc[0x1144 - 0x10dc];
    int body;                // +0x1144; returned by BIFunc_MPGetPlayerBody.
    unsigned char unknown_1148[0x1150 - 0x1148];
    ClipSettings clip;       // +0x1150
    unsigned char unknown_12b4[0x1410 - 0x1150 - sizeof(ClipSettings)];
    CHeadsUpDisplay hud;     // +0x1410
    unsigned char unknown_1be0[0x1c28 - 0x1410 - sizeof(CHeadsUpDisplay)];
    int letterbox;           // +0x1c28; set by BIFunc_DisplayLetterbox.
    unsigned char unknown_1c2c[0x1cf0 - 0x1c2c];
    LookAtTarget lookAt; // +0x1cf0
    void MoveOnPath(CAISplinePath *, CAISplinePath *, float);
    void StopPath();

    void SetCameraShake(float, float, float, float);
    void StartCameraShake(float, float);
    void StopCameraShake(float);
    void StartBackgroundCameraShake(float, float);
    void StopBackgroundCameraShake(float);
    void DoMotionShake(float, float);

    CWeapon *GetCurrentWeapon() const;
    void FireMyWeapon(float);
    void SetFallingDamageState(bool);
    void HandleLookatTrigger(BSObject *, float, float);
    CPlayerWeaponObject *GetCurrentPlayerWeapon() const;
    CPlayerWeaponObject *GetWeaponByCRC(int) const;
    void SetUsingMountedMachineGun(bool, CStaticObject *);
    void SetHideWeapon(bool);
    void FreeBinocularOverlay();
    void SetZoom(float);
    void Respawn();
    int CanPlayerSwitchWeapon();
    int IsPlayerCrouching() const;
    int IsInvincible() const;
    void StartDeathSequence();
    void DropMPDumpObject();
    int CycleWeapon(int, int);
    int GetNumGrenades() const;
    void ThrowGrenade(float);
    void ResetGrenadeCount(int);
    void SetDetonationTimeForPlayerTurret(float);
    int SetWeaponEnable(int, bool);
};

#endif
