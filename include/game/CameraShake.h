// AI-assisted scoped reconstruction from GR8E69; see docs/CameraShake.md and docs/Paths.md.
#ifndef GAME_CAMERA_SHAKE_H
#define GAME_CAMERA_SHAKE_H
#pragma interface
#include "CMatrix.h"
#include "SceneNode.h"
#include "Weapon.h"
#include "CQuaternion.h"
class CAISplinePath;
class CVolSphere;
struct BulletExtentEntry;
class CScreen;
class CCamera;
class CAIObject;
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
    void SetTarget(const char *);
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
    ~CSprite();
    int loaded;  // +0x00; zero when the load failed. Name inferred.
    unsigned char unknown_4[0x1c - 4];
    int field_1c; // Cleared by BIFunc_UseBinoculars.
    int field_20; // Cleared by BIFunc_UseBinoculars.
};
// Method-only view of the multiplayer player body (CPlayerObject +0x1144).
namespace EALA { namespace Character { class Skeleton { public: int FindJoint(const char *) const; }; class AimModifier { public: void SetEnabled(bool); }; } }
class CVolBox;
// Spacing observed; the type and member names are descriptive.
struct AnimatedPlayerRecord {
    unsigned char unknown_0[8];
    int value;
    unsigned char unknown_c[24 - 12];
    void Clear() { value = 0; }
};
class CAnimatedPlayer {
  public:
    unsigned char unknown_0[0x20];
    BulletExtentEntry *extentMinX, *extentMaxX, *extentMinY, *extentMaxY; // +0x20; collision-sort entries, as CBullet's.
    unsigned char unknown_30[0xbc - 0x30];
    CMatrix *matrices;  // +0xbc; the skeleton's joint matrices. Name from Frontline.
    unsigned char unknown_c0[0xd0 - 0xc0];
    CMatrix transform;  // +0xd0; as CAnimObject's.
    CVolBox *localBox;  // +0x110; volume type 3.
    void *localVolume;  // +0x114
    void *worldBox;     // +0x118
    void *worldVolume;  // +0x11c
    BSObject *script;   // +0x120
    unsigned char unknown_124[0x128 - 0x124];
    CVolSphere *sphere; // +0x128; its center is cleared by AttemptUpdate. Name from Frontline.
    unsigned char unknown_12c[0x158 - 0x12c];
    float yaw;          // +0x158
    unsigned char unknown_15c[0x190 - 0x15c];
    CVector3 lastPosition; // +0x190; names of these update fields are from Frontline.
    CVector3 velocity;     // +0x1a0
    unsigned char unknown_1b0[0x1d0 - 0x1b0];
    float lastYaw;         // +0x1d0
    unsigned char unknown_1d4[0x1e0 - 0x1d4];
    CVector3 bonePosition; // +0x1e0
    CQuaternion boneRotation; // +0x1f0
    CVector3 nextBonePosition; // +0x200
    CQuaternion nextBoneRotation; // +0x210
    int field_220;         // +0x220
    unsigned char unknown_224[0x238 - 0x224];
    int attached;          // +0x238; as CAnimObject's. AttemptUpdate leaves the transform alone while set.
    int attachedToLocation; // +0x23c; ... and then takes it from attachLocation.
    unsigned char unknown_240[0x250 - 0x240];
    CMatrix attachLocation; // +0x250
    unsigned char unknown_290[0x344 - 0x290];
    AnimatedPlayerRecord records[3]; // +0x344; AttemptUpdate clears each word at +8.
    unsigned char unknown_38c[0xb84 - 0x38c];
    CWeapon *weapons[4]; // +0xb84; searched by GetWeaponSlot.
    unsigned char unknown_b94[0xf20 - 0xb94];
    int currentWeapon;  // +0xf20; indexes weapons.
    unsigned char unknown_f24[0x1080 - 0xf24];
    int field_1080;     // +0x1080; 0 while bit 29 of the player's flags is set, otherwise 1.
    unsigned char unknown_1084[0x1138 - 0x1084];
    EALA::Character::AimModifier aimModifier; // +0x1138
    unsigned char unknown_1139[0x157c - 0x1139];
    int turning;        // +0x157c; set between script events 299 and 300 by PostUpdate.
    unsigned char unknown_1580[0x15a4 - 0x1580];
    int team;              // +0x15a4; the player's team word, set by InitMultiplayer.
    unsigned char unknown_15a8[0x1610 - 0x15a8];
    CPlayerObject *player; // +0x1610
    float flocoHeading; // +0x1614; the heading PostUpdate turns toward the player's.
    float flocoRate;    // +0x1618
    int flocoCount;     // +0x161c
    void BeginUpdate(float);
    void AttemptUpdate(float);
    void PostUpdate(float);
    void HandleBulletCollision(CBullet *, const CCollision &);
    void Draw(CDrawContext &);
    void UpdateAnimation(float);
    void CommitUpdate();
    int GetLocalBoundingVolume(ISceneNode::EVolumeType) const;
    int GetWorldBoundingVolume(ISceneNode::EVolumeType) const;
    void UpdateLocalBoundingVolume(ISceneNode::EVolumeType, const CVector3 &, const CVector3 &);
    void SetPlayer(CPlayerObject *);
    void SetFlocoParams(float, int);
    void GetFlocoParams(float &, int &);
    int GetWeaponSlot(CWeapon *);
    void EnableAimModifier(bool);
    int LineAlmostCollided(CBullet *) const;
    void SetScript(BSObject *);
    void InitAI();
    void UpdateAI(float);
    CAnimatedPlayer *AsAnimatedPlayerObject();
    const CAnimatedPlayer *AsAnimatedPlayerObject() const;
};
// The subobject at +0x38 of the player's weapon; slot 2 returns its skeleton. The
// class name is descriptive; the slot's name is inferred from its use.
class WeaponAnimOwner {
  public:
    virtual void unknown1();
    virtual const EALA::Character::Skeleton *GetSkeleton();
};
class CPlayerWeaponObject {
  public:
    unsigned char unknown_0[0x38];
    WeaponAnimOwner animOwner; // +0x38; the animated-object subobject whose slot 2 returns the skeleton.
    unsigned char unknown_3c[0xc8 - 0x3c];
    int field_c8;      // +0xc8; the draw command's sort word.
    unsigned char unknown_cc[0xd0 - 0xcc];
    CMatrix transform; // +0xd0; bound while the weapon draws. Layout observed.
    unsigned char unknown_110[0x2d8 - 0x110];
    void *field_2d8;   // +0x2d8; Draw complains and skips while it is null.
    unsigned char unknown_2dc[0x2e0 - 0x2dc];
    CPlayerWeaponObject *previous; // +0x2e0
    CPlayerWeaponObject *next; // +0x2e4
    CPlayerObject *player; // +0x2e8
    CWeapon *weapon; // +0x2ec
    CSprite *aimOverlay;   // +0x2f0
    CSprite *crosshair;    // +0x2f4
    void *muzzleFlash;     // +0x2f8
    unsigned char unknown_2fc[0x370 - 0x2fc];
    CVector3 field_370; // +0x370; cleared by Init.
    unsigned char unknown_380[0x38c - 0x380];
    float recoilYaw;   // +0x38c; added to the player's by FireMyWeapon.
    float recoilPitch; // +0x390
    unsigned char unknown_394[0x3a8 - 0x394];
    int muzzleJoint;   // +0x3a8; the "jo_fire" joint found by GetBulletEmitterMatrix.
    unsigned char unknown_3ac[0x3b0 - 0x3ac];
    int state;       // +0x3b0; set to 3 by BIFunc_AddPlayerAmmo, stored by SetDrawDelay.
    char weaponName[0x3d2 - 0x3b4]; // +0x3b4; set by SetWeaponName (extent unknown).
    char name[0x3f0 - 0x3d2]; // +0x3d2; animation folder name (extent unknown).
    union {
        unsigned int flags; // +0x3f0; bit 29 set by BIFunc_AddPlayerAmmo before DoneReloading.
        struct {
            unsigned int flag31 : 1;
            unsigned int visible : 1; // Bit 30; cleared when the player dies. Name inferred.
            unsigned int enabled : 1; // Bit 29; SetWeaponEnable. Name inferred.
            unsigned int flag28 : 1;
        };
    };
    int field_3f4;     // +0x3f4; follows the weapon's reserve ammunition outside multiplayer.
    // Original nested enum tag; member names are descriptive.
    enum Partition { PartitionDefault = 0, PartitionAlternate = 1 };
    static CPlayerWeaponObject *m_pPlayerWeaponFirst;
    void FreeAimOverlay();
    void FreeCrosshair();
    void Shutdown();
    void Shoot(EWeaponShootType, int);
    void AddAmo(int);
    void SetAmmo(unsigned int, unsigned int);
    unsigned int IsDrawEnabled() const;
    int GetBulletEmitterMatrix();
    void SetAnimFolder(const char *);
    void SetWeaponName(const char *);
    void Destroy();
    void Init();
    void Init(int, int, CPlayerObject *);
    void BeginUpdate(float);
    void AttemptUpdate(float);
    void CommitUpdate();
    void Draw(CDrawContext &);
    static void DrawWeapon(CDrawContext &, void **);
    void UpdateWeaponTransforms();
    void UpdateMuzzleFlash();
    void SetDrawDelay(int);
    CPlayerWeaponObject *AsPlayerWeaponObject();
    const CPlayerWeaponObject *AsPlayerWeaponObject() const;
    void CreateEjectedShell(int);
    void DoneReloading();
    void SetAnimationState(const char *, int, Partition);
    bool IsSelectable() const;
};
// Scoped view of the mounted gun the player holds; names are descriptive.
struct MountedGunView {
    unsigned char unknown_0[0x210];
    float zoomFov;           // +0x210
    unsigned char unknown_214[0x218 - 0x214];
    unsigned int flags;      // +0x218; bit 19 enables the zoom limit.
};
// Draw view reached through +0xd4; only the slot-3 use is observed.
class PlayerDrawView {
  public:
    virtual void unknown1();
    virtual void unknown2();
    virtual void Submit(void *, const CMatrix &);
};
struct BS_STRUCT_HitByBulletInfo_struct;
struct SMPPlayerData;
class CPlayerObject {
  public:
    static void *operator new(unsigned int);
    static void operator delete(void *);
    void SetPlayersMountedMGPositions(CMatrix *);
    // Member-only view of the target search at +0x1ca0; the class name is original,
    // member names and the inline's name are descriptive.
    class TargetFind {
      public:
        void Enable(CPlayerObject *);
        int AddTarget(ISceneNode *, CVector3 &, float, unsigned char);
        void ChooseTarget();
        void ClearTargets();
        unsigned char unknown_0[8];
        ObserverLinkBase link;   // +0x08
        unsigned char unknown_10[4];
        int field_14;            // +0x14
        unsigned char unknown_18[0x40 - 0x18];
        int field_40;            // +0x40
        int field_44;            // +0x44
        unsigned char unknown_48[0x50 - 0x48];
        void Disable() {
            field_44 = 0;
            link.Unlink();
            field_14 = 0;
            field_40 = 0;
        }
    };
    // Member-only view of the embedded look-at controller at +0x1cf0.
    class LookAtTarget {
      public:
        unsigned char unknown_0[8];
        ObserverLinkBase link;   // +0x08
        unsigned char unknown_10[4];
        ISubject *subject;       // +0x14
        unsigned char unknown_18[0x20 - 0x18];
        CVector3 target;         // +0x20
        float time;              // +0x30
        int active;              // +0x34
        void Enable(ISceneNode *, float);
        void Enable(const CVector3 &, float);
        void Disable();
    };
    // 36 bytes: eight floats, then the table pointer at +32 (the class declares its
    // data before its virtual destructor). Field names are descriptive.
    // Member-only view with the same observer prefix as TargetFind; the name is original.
    class AimAdjust {
      public:
        unsigned char unknown_0[8];
        ObserverLinkBase link;   // +0x08
        unsigned char unknown_10[4];
        ISubject *subject;       // +0x14
        unsigned char unknown_18[0x20 - 0x18];
        CVector3 target;         // +0x20
        CVector3 lastTarget;     // +0x30
        bool HasSubject() const { return subject != 0; }
        int field_40;            // +0x40
        int active;              // +0x44
        void Enable();
        void UpdateTarget(const CCamera &);
        void UpdatePlayer(float &, float &, const CCamera &);
    };
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
        bool IsActive() const { return rate != 0.0f || current != 0.0f; } // Name inferred.
        CCameraShake();
        virtual ~CCameraShake();
        void SetShake(float, float, float, float);
        void SetShake(float, float);
        void Evaluate(float &, float &);
        void Update(float);
    };

    // Member-only view; unknown bytes retain their original ownership.
    struct Flags {
        unsigned int pitchWhileMounted : 1; // Bit 31; name inferred from SetTMLocalToWorld.
        unsigned int hideWeapon : 1;    // Bit 30: IsDrawEnabled answers no while set; name inferred.
        unsigned int headingOverride : 1; // Bit 29: GetHeading reads +0x1c8c; name inferred.
        unsigned int fallingDamage : 1; // Bit 28: SetFallingDamageState.
        unsigned int flag27 : 1;
        unsigned int canCrouch : 1;     // Bit 26: BIFunc_PlayerCanCrouch.
        unsigned int motionShake : 1;   // Bit 25: set by DoMotionShake for a positive amount.
        unsigned int flag24 : 1;
        unsigned int wielding : 1;      // Bit 23: BIFunc_SetPlayerWielding.
        unsigned int triggerChecks : 1; // Bit 22: BIFunc_TogglePlayerVsTriggerChecks.
        unsigned int superAimMode : 1;  // Bit 21: BIFunc_PlayerSuperAimMode.
        unsigned int bit20 : 1;
        unsigned int bit19 : 1;
        unsigned int bit18 : 1;
        unsigned int bit17 : 1;
        unsigned int unknown_16_15 : 2;
        unsigned int bit14 : 1;
        unsigned int flag13 : 1;
        unsigned int flag12 : 1;
        unsigned int invincible : 1;    // Bit 11: BIFunc_MakePlayerInvincible.
        unsigned int unknown_low : 11;
    };
    struct PathFlagBits {
        unsigned int unknown_31 : 1;
        unsigned int peekAllowed : 1;   // Bit 30; set for a floor contact too.
        unsigned int bit29 : 1;         // Bit 29; a shallower contact.
        unsigned int bit28 : 1;
        unsigned int bit27 : 1;
        unsigned int bit26 : 1;
        unsigned int bit25 : 1;
        unsigned int binoculars : 1;    // Bit 24: BIFunc_UseBinoculars.
        unsigned int climbing : 1;      // Bit 23: SetClimbingState.
        unsigned int crouched : 1;      // Bit 22; names from Frontline.
        unsigned int crouching : 1;     // Bit 21.
        unsigned int peeking : 1;       // Bit 20.
        unsigned int bit19 : 1;
        unsigned int bit18 : 1;
        unsigned int bit17 : 1;
        unsigned int climbCleared : 1;  // Bit 16: cleared when climbing starts.
        unsigned int meleeImmune : 1;   // Bit 15: BIFunc_CanMeleeTarget returns 0.
        unsigned int noShells : 1;      // Bit 14: suppresses BIFunc_PlayerEjectShell.
        unsigned int peekStarted : 1;   // Bit 13.
        unsigned int bit12 : 1;
        unsigned int bit11 : 1;
        unsigned int bit10 : 1;
        unsigned int mmgBlocked : 1;    // Bit 9: skips BIFunc_AttachPlayerToMountedMachineGun in multiplayer.
        unsigned int unknown_8_0 : 9;
    };
    unsigned char unknown_0[0x20];
    BulletExtentEntry *extentMinX, *extentMaxX, *extentMinY, *extentMaxY; // +0x20; collision-sort entries, as CBullet's.
    unsigned char unknown_30[0x38 - 0x30];
    unsigned char aiDoodad[0x44 - 0x38]; // +0x38; storage of the CAIPlayerDoodad member.
    CAIObject *aiObject;     // +0x44
    unsigned char unknown_48[0x50 - 0x48];
    CMatrix localToWorld;    // +0x50
    CMatrix cameraMatrix;    // +0x90
    void *localBox;          // +0xd0; volume type 3. IsVisible transforms it into worldBox.
    PlayerDrawView *worldBox; // +0xd4
    unsigned char unknown_d8[0xe0 - 0xd8];
    unsigned char boxStorage[0x180 - 0xe0]; // +0xe0; the CVolBox the player's box is, kept as raw storage (world box after it).
    unsigned char localVolume[0x1d0 - 0x180]; // +0x180; the volume for types 0-2, a CVolCapsule.
    unsigned char worldVolume[0x10];          // +0x1d0; extent runs on through bounds.
    // Name and accessors inferred; GetCentroid returns the midpoint.
    struct Bounds {
        CVector3 min, max;
        CVector3 GetMin() const { return min; }
        CVector3 GetMax() const { return max; }
    } bounds;                // +0x1e0
    unsigned char unknown_200[0x220 - 0x200];
    BSObject *script;        // +0x220
    unsigned char unknown_224[0x230 - 0x224];
    CVector3 velocity;       // +0x230
    CVector3 acceleration;   // +0x240; cleared with the velocity by Halt.
    CVector3 force;          // +0x250; written by ApplyForceTo.
    CVector3 position;       // +0x260
    float field_270;         // +0x270
    float yaw;               // +0x274
    float pitch;             // +0x278
    unsigned char unknown_27c[0x284 - 0x27c];
    float field_284;         // +0x284
    float field_288;         // +0x288
    float field_28c;         // +0x28c; cleared by Halt.
    unsigned char constraints[0xd40 - 0x290]; // +0x290; a CClsnConstraintsBase first (extent inferred).
    float field_d40;         // +0xd40
    union {
        unsigned int pathFlags; // +0xd44; bit 19 enables path movement, bit 27 is set while aiming.
        PathFlagBits pathBits;
    };
    int field_d48;           // +0xd48
    Flags flags;
    float field_d50;         // +0xd50
    float field_d54;         // +0xd54; positive blocks explosion damage in coop and multiplayer.
    CCameraShake cameraShake;
    CCameraShake backgroundShake;
    float motionShakeAmount, motionShakeRate, motionShakeTime;

    unsigned char unknown_dac[4];
    CVector3 cameraOffset;   // +0xdb0; SetTMLocalToWorld builds the camera matrix from it.
    float field_dc0;         // +0xdc0
    unsigned char unknown_dc4[0xde0 - 0xdc4];
    CVector3 peekBase;       // +0xde0; names from Frontline.
    CVector3 peekRight;      // +0xdf0
    CAISplinePath *movePath, *lookPath; // +0xe00, +0xe04
    float pathParameter, pathRate; // +0xe08, +0xe0c
    float field_e10;         // +0xe10
    unsigned char unknown_e14[0xe2c - 0xe14];
    float field_e2c;         // +0xe2c; the heading CAnimatedPlayer::PostUpdate takes while melee immune.
    float fov;               // +0xe30; set by BIFunc_SetGameFOV.
    float targetFov;         // +0xe34; the clamped value SetZoom passes to TransitionFOV.
    float fovTarget;         // +0xe38; set by TransitionFOV.
    float fovRate;           // +0xe3c
    int fovMode;             // +0xe40
    unsigned char unknown_e44[0xe48 - 0xe44];
    CSprite *binocularOverlay; // +0xe48; set by BIFunc_UseBinoculars.
    float recoilYaw;         // +0xe4c; accumulated by FireMyWeapon.
    float recoilPitch;       // +0xe50
    float peekYaw;           // +0xe54; the yaw kept by PeekLeftRight.
    float field_e58;         // +0xe58
    float field_e5c;         // +0xe5c
    unsigned char weaponMap[0x10]; // +0xe60; a map<int, CPlayerWeaponObject *> in the original.
    CPlayerWeaponObject *currentWeapon; // +0xe70
    CPlayerWeaponObject *weaponList; // +0xe74; the first of a ring linked through CPlayerWeaponObject::next.
    CWeapon *thrownWeapon; // +0xe78; checked by BIFunc_PlayerWeaponInfo when no weapon object matches.
    unsigned char unknown_e7c[0xe80 - 0xe7c];
    CStaticObject *mmgBase; // +0xe80; set by BIFunc_AttachHierBaseToMMG.
    MountedGunView *mountedGun; // +0xe84; SetZoom reads its zoom limit.
    int field_e88;           // +0xe88
    int field_e8c;           // +0xe8c
    int field_e90;           // +0xe90
    CWeapon *turretWeapon;   // +0xe94; SetDetonationTimeForPlayerTurret writes through it.
    CWeapon *mountedWeapon;  // +0xe98; used while pathBits bit 15 is set.
    unsigned char unknown_e9c[0xea0 - 0xe9c];
    CMatrix mountedOffset;   // +0xea0; the gun's mount transform, replaced by mountedOffsetAlt while bits 18 and 17 are set.
    CMatrix mountedPlayerOffset; // +0xee0; applied to the player's transform by SetPlayersMountedMGPositions.
    unsigned char unknown_f20[0xf60 - 0xf20];
    CMatrix mountedOffsetAlt; // +0xf60
    unsigned char unknown_fa0[0xfc0 - 0xfa0];
    EClsnId collisionId;     // +0xfc0
    unsigned char unknown_fc4[0x1000 - 0xfc4];
    int field_1000;          // +0x1000
    unsigned char unknown_1004[0x1090 - 0x1004];
    BSObject *lookatTrigger; // +0x1090; set by HandleLookatTrigger, as the next two.
    float lookatAngle;       // +0x1094; radians, from the trigger's degrees.
    float lookatValue;       // +0x1098
    unsigned char unknown_109c[0x10a0 - 0x109c];
    CVector3 field_10a0;     // +0x10a0
    unsigned char unknown_10b0[0x10bc - 0x10b0];
    int cameraPitchUnhinged; // +0x10bc
    unsigned short surfaceSound; // +0x10c0; name from Frontline.
    short field_10c2;        // +0x10c2
    float field_10c4;        // +0x10c4
    float field_10c8;        // +0x10c8; the height OnCollision passes to UpdateHeight.
    float field_10cc;        // +0x10cc
    int field_10d0;          // +0x10d0
    char mountedShotSide;    // +0x10d4; PostUpdate alternates its low bits while bit 18 of the flags is set.
    unsigned char unknown_10d5[0x10d8 - 0x10d5];
    union {
        int viewWord;        // +0x10d8; compared with the draw binding's owner by CPlayerWeaponObject::Draw.
        struct {
            short viewWordHigh; // The word here compares to a draw view's owner in CAnimatedPlayer::Draw.
            short promptSlot;   // +0x10da; prompt buffer used by BIFunc_AddPlayerAmmo.
        };
    };
    int playerIndex;         // +0x10dc; read as a word and as its low byte.
    unsigned char unknown_10e0[0x112c - 0x10e0];
    int field_112c;          // +0x112c
    float field_1130, field_1134, field_1138, field_113c; // +0x1130
    int field_1140;          // +0x1140
    int body;                // +0x1144; returned by BIFunc_MPGetPlayerBody.
    unsigned char unknown_1148[0x1150 - 0x1148];
    ClipSettings clip;       // +0x1150
    unsigned char unknown_12b4[0x1410 - 0x1150 - sizeof(ClipSettings)];
    CHeadsUpDisplay hud;     // +0x1410
    unsigned char unknown_1be0[0x1c28 - 0x1410 - sizeof(CHeadsUpDisplay)];
    int letterbox;           // +0x1c28; set by BIFunc_DisplayLetterbox.
    unsigned char unknown_1c2c[0x1c72 - 0x1c2c];
    char name[0x1c80 - 0x1c72]; // +0x1c72; returned by GetName.
    union {
        int teamWord;        // +0x1c80; written whole by InitMultiplayer.
        struct {
            unsigned char unknown_1c80[3];
            unsigned char team; // +0x1c83
        };
    };
    unsigned char unknown_1c84[4];
    int fireDelay;           // +0x1c88; set from RESPAWN_FIRE_DELAY. Name from Frontline.
    union {
        float mountedHeading; // +0x1c8c; GetHeading's answer while bit 29 of the flags is set.
        float deathYaw;       // The yaw when the death sequence started (same word).
    };
    float deathPitch;        // +0x1c90
    unsigned char unknown_1c94[0x1ca0 - 0x1c94];
    TargetFind targetFind;   // +0x1ca0
    LookAtTarget lookAt; // +0x1cf0
    void GetTMLocalToWorld(CMatrix &) const;
    int GetLocalBoundingVolume(ISceneNode::EVolumeType) const;
    int GetWorldBoundingVolume(ISceneNode::EVolumeType) const;
    void CommitUpdate();
    int IsVisible(CDrawContext &) const;
    unsigned int IsDrawEnabled() const;
    EClsnId GetCollisionId() const;
    void Halt();
    void PreTransform(const CMatrix &);
    void Transform(const CMatrix &);
    void SetPosition(const CVector3 &);
    void SetBasis(const CVector3 &, const CVector3 &, const CVector3 &);
    void Move(const CVector3 &);
    void Rotate(const CVector3 &, float);
    void Pitch(float);
    void Roll(float);
    void Orthonormalize();
    void GetPosition(CVector3 &) const;
    void GetCentroid(CVector3 &) const;
    void GetRightward(CVector3 &) const;
    void GetForward(CVector3 &) const;
    void GetUpward(CVector3 &) const;
    void ApplyForceTo(const CVector3 &, float);
    int GetCurrentWeaponClipAmoCount() const;
    int GetCurrentWeaponReserveAmoCount() const;
    void TriggerScriptEvent(int, void *, bool);
    void SetScript(BSObject *);
    void Reset();
    void EnterLightVolume(BPDLightVolume *);
    void ExitLightVolume(BPDLightVolume *);
    void SetLightVolumeTransitionDuration(float);
    void UpdateAI(float);
    float GetHitpoints() const;
    void ReInit();
    void Init();
    void InitAI();
    CAIObject *GetAIObject();
    void CleanUp();
    static void InitClass(CScreen *, CCamera *);
    void BeginStand();
    void BeginCrouch();
    void UpdateDeathSequence(float);
    void PeekLeftRight();
    void ShakeCamera(float &, float &, float);
    void OnCollision(const CCollision &);
    void HitPickup(CStaticObject *);
    void UpdateHeight(float);
    void StartPlayerCrouched();
    void PeekUp(float);
    void DuckDown(float);
    float GetHeading();
    int GetScriptObject() const;
    CAIDoodad *GetAIDoodad();
    const CAIDoodad *GetAIDoodad() const;
    CPlayerObject *AsPlayerObject();
    const CPlayerObject *AsPlayerObject() const;
    void SetCollisionId(EClsnId);
    CVector3 GetWorldLinearVelocity() const;
    const char *GetName();
    unsigned char GetPlayerIndex();
    int GetTeam();
    void Destroy();
    void SetClimbingState(bool);
    void TransitionFOV(float, float);
    void MoveOnPath(CAISplinePath *, CAISplinePath *, float);
    void UpdateMovePath(float);
    void *GetCurrentBulletSprite() const;
    CPlayerWeaponObject *AddPlayerWeapon(int);
    void StoreSaveData();
    void RestoreSaveData();
    void PostUpdate(float);
    void EjectMountedMGShell();
    void SetFinishedLevelData();
    void UpdateCollisionVolumes();
    int Constrain(float);
    void Yaw(float);
    void SetTMLocalToWorld(const CMatrix &);
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
    void SetHealthMax();
    void HandleBulletCollision(CBullet *, const CCollision &);
    void InitMultiplayer(SMPPlayerData *, bool);
    int CanPlayerSwitchWeapon();
    bool IsPlayerCrouching() const;
    int IsInvincible() const;
    void SetMPShotLocation(BS_STRUCT_HitByBulletInfo_struct *);
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
