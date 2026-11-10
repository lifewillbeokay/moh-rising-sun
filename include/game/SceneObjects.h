// AI-assisted scoped reconstruction from GR8E69; see docs/Script.md.
#ifndef GAME_SCENE_OBJECTS_H
#define GAME_SCENE_OBJECTS_H
#include "SceneNode.h"
#include "AITargeting.h"
#include "CMatrix.h"
#include "CQuaternion.h"
#include "LightVolumeManager.h"
#include "CVector3.h"
struct BSObject;
class IVolume;
class CWeapon;
// Original enum tag; its members are unknown.
enum EBSEventEnum { BSEventUnknown = 0 };

// Original enum tag; its members are unknown.
enum EPlayerCollisionType { PlayerCollisionTypeUnknown = 0 };
EClsnId GetCollisionIDFromScriptCollisionType(EPlayerCollisionType);
enum EScriptCollisionType { ScriptCollisionTypeUnknown = 0 };
EClsnId GetCollisionIDFromScriptCollisionType(EScriptCollisionType, ETeam);
// Original enum tag; its members are unknown.
enum EFireMethod { FireMethodUnknown = 0 };
enum EExpressionType { ExpressionNone = -1 }; // Named by CSoldierObject::SetExpression's symbol; members inferred.

// Member-only views of scene-object classes reached through the ISceneNode
// casts. Each view starts at the ISceneNode subobject (the casts return it
// unadjusted) and names only the observed members; bases, virtual interfaces
// and complete sizes are not declared. Built-ins that need an inherited virtual
// convert the view back to ISceneNode without adjustment.
// The soldier's embedded jaw controller. Member names are descriptive inferences
// from the reconstructed methods.
enum EJawType { JawTypeUnknown = 0 };
class CJawController {
  public:
    CJawController();
    void Update(float);
    void SetMotionType(EJawType);
    float closeStart; // +0x00; timer value at which the closing phase starts.
    float closeEnd;   // +0x04; timer value above which the jaw is at rest.
    float riseTime;   // +0x08
    float holdTime;   // +0x0c
    float fallTime;   // +0x10
    int cyclesLeft;   // +0x14
    int cycleCount;   // +0x18
    float minValue;   // +0x1c
    float maxValue;   // +0x20
    int previousCode; // +0x24; meaning unknown.
    int currentCode;  // +0x28; meaning unknown.
    float timer;      // +0x2c
    float value;      // +0x30
    float startDelay; // +0x34
    int delaying;     // +0x38
    int type;         // +0x3c
};

struct MotionData;
MotionData *InitMotionData(const char *);
class CBullet;
class CLight;
class CCollision;
class CStaticMesh;
class CTankObject;
struct MotionFrame {
    CVector3 position;
    CQuaternion rotation;
};
// Method-only view; the storage it occupies in an object is not established.
class CObjectFadeManager {
  public:
    CObjectFadeManager();
    void StartFadeOut(float, float);
    void StartFadeIn(float, float);
    float FadeValue() const;
    int AreFading();
    void Update(float);
    int HalfDone();
    float GetShadowAlphaScale();

    int state;   // 0 idle, 1 fading in, 2 fading out.
    float time;  // +0x04
    float duration;
    float delay; // +0x0c: fraction of the duration before the fade starts.
};
class CStaticObject {
  public:
    // Descriptive inline tests of the script-motion bits.
    int ScriptMoving() const { return flags.scriptMoving; }
    int ScriptRotating() const { return flags.scriptRotating; }
    int ScriptScaling() const { return flags.scriptScaling; }
    void SetLocalBoundingVolume(ISceneNode::EVolumeType, IVolume *);
    void SetWorldBoundingVolume(ISceneNode::EVolumeType, IVolume *);
    IVolume *GetLocalBoundingVolume(ISceneNode::EVolumeType) const;
    IVolume *GetWorldBoundingVolume(ISceneNode::EVolumeType) const;
    void InitMotionProperties(CStaticMesh *, int, int, int, int, int, MotionFrame *);
    void Init();
    float CalculateMovementTime(const CVector3 &, const CVector3 &);
    void ScaleFromScript(float, float, EBSEventEnum);
    void CommitAnimMotionUpdate();
    void CommitScriptMotionUpdate();
    void AnimUpdate(float);
    void SetAttachedLight(CLight *, const CVector3 &);
    void HandleBulletCollision(CBullet *, const CCollision &);
    void TranslateFromScript(const CVector3 &, const CVector3 &, EBSEventEnum);
    void BeginScriptMotionUpdate(float);
    void BeginAnimMotionUpdate(float);
    void AttemptScriptMotionUpdate(float);
    void AttemptAnimMotionUpdate(float);
    void AttemptCollisionDamage(const CCollision &);
    void BeginPathMotionUpdate(float);
    void AttemptPathMotionUpdate(float);
    void CommitPathMotionUpdate();
    void UpdatePathMotionDestAndSpeed();
    void Destroy();
    void TriggerScriptEvent(unsigned short, void *, bool);
    void InitAim(bool, float);
    float FadeValue() const;
    void SerializeSaveData();
    void SetScript(BSObject *);
    void SetCollisionId(EClsnId);
    void SetDrawEnabled(bool);
    int GetObjectType() const;
    CVector3 GetWorldLinearVelocity() const;
    CStaticObject *AsStaticObject();
    const CStaticObject *AsStaticObject() const;
    BSObject *GetScriptObject() const;
    unsigned int IsDrawEnabled() const;
    void SetLightVolumeTransitionDuration(float);
    void *AsThrownObject();
    void *AsWeaponObject();
    CLight *GetAttachedLight() const;
    CTankObject *AsHierTankObject();
    EClsnId GetCollisionId() const;
    void GetTMLocalToWorld(CMatrix &) const;
    void GetPosition(CVector3 &) const;
    void GetRightward(CVector3 &) const;
    void GetForward(CVector3 &) const;
    void GetUpward(CVector3 &) const;
    void SetTMLocalToWorld(const CMatrix &);
    void SetPosition(const CVector3 &);
    void SetBasis(const CVector3 &, const CVector3 &, const CVector3 &);
    void EnterLightVolume(BPDLightVolume *);
    void ExitLightVolume(BPDLightVolume *);
    BPDLightVolume *GetLightVolume();
    void MarkForDestruction(int);
    void Draw(CDrawContext &);
    static void ExecuteDraw(CDrawContext &, void **);
    void DrawDebugInfo(CDrawContext &);
    void BeginUpdate(float);
    int IsVisible(CDrawContext &) const;
    void AttemptUpdate(float);
    void CommitUpdate();
    void StartMotionPlayback(int, int, EBSEventEnum);
    void StopMotionPlayback();
    void ModifyMotionData(MotionData *);
    void SetMovementPath(int, bool, bool, float, float, float, float, BSObject *, float, bool);
    void ResumeMotionPlayback();
    void PlayAnimation(int, int, int, EBSEventEnum);
    void FadeIn(float);
    void FadeOut(float);
    struct Flags {
        unsigned int drawEnabled : 1;   // Bit 31.
        unsigned int scriptMoving : 1;  // Bit 30; descriptive, Frontline's reading.
        unsigned int scriptRotating : 1; // Bit 29; descriptive.
        unsigned int scriptScaling : 1; // Bit 28; set by ScaleFromScript.
        unsigned int hasMotion : 1;     // Bit 27; descriptive.
        unsigned int motionPlaying : 1; // Bit 26: cleared by StopMotionPlayback.
        unsigned int playReverse : 1;   // Bit 25; set by PlayAnimation on a backward range.
        unsigned int allowCollisionDamage : 1; // Bit 24.
        unsigned int pathFlag1 : 1;            // Bit 23: BIFunc_MechSetPathFlag selector 1.
        unsigned int dumpObject : 1;    // Bit 22; Destroy hands the object to DestroyDumpObject.
        unsigned int attached : 1;      // Bit 21; AttemptUpdate skips script motion. Descriptive.
        unsigned int unknown_20 : 1;
        unsigned int aimEnabled : 1;    // Bit 19; InitAim. Descriptive.
        unsigned int unknown_low : 19;
    };
    // The four sort-list nodes at +0x20; each keeps a bound as a float at +8. Names descriptive.
    struct SortNode {
        unsigned char unknown_0[8];
        float value;
    };
    unsigned char unknown_0[0x20];
    SortNode *sortMinX, *sortMaxX, *sortMinY, *sortMaxY; // +0x20
    unsigned char unknown_30[0x4c - 0x30];
    void *model;          // +0x4c; drawing is skipped while it is null.
    CMatrix transform;    // +0x50
    unsigned char unknown_90[0xa0 - 0x90];
    unsigned char lookTraversal[0x30]; // +0xa0; a CAISplinePathTraversal.
    unsigned char pathTraversal[0x30]; // +0xd0; a CAISplinePathTraversal.
    float pathSpeed;      // +0x100; the current speed, copied to +0x230 on commit. Descriptive.
    float pathField104;   // +0x104; SetMovementPath stores the speed argument here too.
    float pathMaxSpeed;   // +0x108; name from Frontline.
    float pathAcceleration; // +0x10c; descriptive.
    float pathTurnSlowdown; // +0x110; share of the speed lost at full yaw rate; BIFunc_MechSetPathFlag stores value / 100. Descriptive.
    void *pathFollowTarget; // +0x114; the player while following.
    float pathFollowDistanceSquared; // +0x118
    float pathYawRate;    // +0x11c; descriptive.
    float pathPitchRate;  // +0x120; descriptive.
    float pathMaxTurnRate; // +0x124; descriptive.
    float pathTurnAcceleration; // +0x128; descriptive.
    unsigned int pathState; // +0x12c; cleared by Init. Descriptive.
    IVolume *localVolumeType3; // +0x130; returned for volume type 3.
    // +0x134: GetLocalBoundingVolume's type-0 volume, despite the older field name.
    IVolume *worldVolume; // +0x134; freed by CThrownObject::CleanUp, queried for GetRadius.
    IVolume *worldVolumeType3; // +0x138
    // +0x13c: GetWorldBoundingVolume's type-0 volume; typed int by earlier code.
    int field_13c;        // +0x13c; cleared by CThrownObject::CleanUp.
    BSObject *script;     // +0x140
    unsigned char unknown_144[0x150 - 0x144];
    CVector3 animPosition; // +0x150
    CVector3 motionStartPosition; // +0x160
    CQuaternion motionStartOrientation; // +0x170
    CQuaternion animOrientation; // +0x180
    CVector3 velocity;    // +0x190; CThrownObject limits its speed.
    CVector3 pathPosition; // +0x1a0
    CQuaternion orientation; // +0x1b0; kept in step with the transform.
    CQuaternion scriptRotationDelta; // +0x1c0
    CQuaternion scriptTargetOrientation; // +0x1d0
    float scriptScaleTarget; // +0x1e0
    float scriptScaleBase; // +0x1e4; starts at 1 in Init.
    float scriptMoveTime; // +0x1e8
    float scriptRotateTime; // +0x1ec
    float scriptScaleRate; // +0x1f0
    int scriptMoveEvent;  // +0x1f4; only the low half reaches TriggerScriptEvent.
    int scriptRotateEvent; // +0x1f8
    int scriptScaleEvent; // +0x1fc
    float scriptMoveStep; // +0x200
    float scriptRotateStep; // +0x204
    float scriptScaleStep; // +0x208
    int field_20c;        // +0x20c; the collision id. CThrownObject::LimitSpeed does nothing while -1.
    float aimValue;       // +0x210; InitAim. Descriptive.
    int maxLightPriority; // +0x214
    Flags flags;          // +0x218
    unsigned int mechFlags; // +0x21c; bits set and cleared by BIFunc_MechSetFlag.
    CVector3 pathDest;    // +0x220; steered toward by AttemptPathMotionUpdate. Name from Frontline.
    float pathPreviousSpeed; // +0x230; the speed at the start of the update. Descriptive.
    int motionEvent;      // +0x234; PlayAnimation's event.
    int motionFrame;      // +0x238
    int motionEnd;        // +0x23c
    int motionFrameCount; // +0x240
    float motionElapsed;  // +0x244
    int motionSpeed;      // +0x248
    int motionStart;      // +0x24c
    int motionPlayEvent;  // +0x250
    int motionPlayStart;  // +0x254
    int motionKey;        // +0x258
    int motionLoop;       // +0x25c
    float motionKeyTime;  // +0x260; time into the current key. Descriptive.
    float motionRate;     // +0x264
    int motionTimer;      // +0x268
    float motionSampleRate; // +0x26c
    float motionRemaining; // +0x270
    float motionStep;     // +0x274
    CQuaternion motionResetOrientation; // +0x278
    float dumpAge;        // +0x288; advances while the object is a dump object; the motion update stores its speed here. Descriptive.
    int motionSpeedKey;   // +0x28c; the key +0x288 was measured for. Descriptive.
    MotionFrame *motionFrames; // +0x290
    unsigned char unknown_294[0x2a8 - 0x294];
    CLight *attachedLight; // +0x2a8; the subject of the weak reference starting at +0x294.
    unsigned char unknown_2ac[0x2b0 - 0x2ac];
    CVector3 attachedLightOffset; // +0x2b0
    CLightVolumeManager lightVolumes; // +0x2c0
    CObjectFadeManager fade; // +0x2dc
    float destructionTimer; // +0x2ec; meaning inferred from the MarkForDestruction call.
};

struct COORD3;
namespace EAGL { class Transform; }
namespace EALA {
class Task;
namespace Character {
struct CollisionData;
// The record a choreographer hands its handler; names beyond the class names are descriptive.
class Opcode {
  public:
    class Handler {
      public:
        virtual int Execute(Opcode &) = 0;
    };
    unsigned short unknown_0;
    unsigned short id;        // +2
    unsigned short paramLow;  // +4
    unsigned short paramHigh; // +6
    unsigned short extra;     // +8
};
}
}
class CAnimatedVolume;
enum EAnimCallback { AnimCallbackUnknown = 0 };
class CScreen;
struct MATRIX4;
// The part of EALA::Character::Mesh that CAnimObject::Draw reads; names descriptive.
struct StaticMeshPart;
struct CharacterMeshView {
    unsigned char unknown_0[12];
    StaticMeshPart *part; // +0xc
    unsigned char unknown_10[0x30 - 0x10];
    int noSortedPass; // +0x30; the second draw command is queued only while this is zero.
};
class CAnimObject {
  public:
    void HandleBulletCollision(CBullet *, const CCollision &);
    static void InitClass(CScreen *);
    void OnCollision(const CCollision &);
    void Init();
    void CommitUpdate();
    void UpdateAnimation(float);
    void DetectGround();
    void BeginUpdate(float);
    void Yaw(float);
    void SetLocalBoundingVolume(ISceneNode::EVolumeType, IVolume *);
    void SetWorldBoundingVolume(ISceneNode::EVolumeType, IVolume *);
    IVolume *GetLocalBoundingVolume(ISceneNode::EVolumeType) const;
    IVolume *GetWorldBoundingVolume(ISceneNode::EVolumeType) const;
    void TriggerScriptEvent(int, void *, bool);
    unsigned int IsDrawEnabled() const;
    EClsnId GetCollisionId() const;
    void Reset();
    void SetPosition(const CVector3 &);
    void Move(const CVector3 &);
    float GetYaw() const;
    void SetYaw(float);
    void Orthonormalize();
    void GetPosition(CVector3 &) const;
    void GetRightward(CVector3 &) const;
    void GetForward(CVector3 &) const;
    void GetUpward(CVector3 &) const;
    void EnterLightVolume(BPDLightVolume *);
    void ExitLightVolume(BPDLightVolume *);
    BPDLightVolume *GetLightVolume();
    void SetTMLocalToWorld(const CMatrix &);
    void GetTMLocalToWorld(CMatrix &) const;
    void AttemptUpdate(float);
    void AttachToLocation(const CMatrix &, float);
    COORD3 GetPosition();
    float GetRotation();
    void SetPosition(const COORD3 &);
    void SetRotation(const float &);
    void MarkForDestruction(int);
    void Destroy();
    void SetScript(BSObject *);
    void SetCollisionId(EClsnId);
    CAnimObject *AsAnimObject();
    const CAnimObject *AsAnimObject() const;
    BSObject *GetScriptObject() const;
    CVector3 GetWorldLinearVelocity() const;
    void Halt();
    void SetAnimCallback(EAnimCallback);
    void StartAnimCallback(const float &);
    void UpdateAnimCallback(const float &);
    void Pathing_cb(const float &);
    void PathingFacing_cb(const float &);
    void Adjusting_cb(const float &);
    void TrackTarget_cb(const float &);
    void OnCycleEnd();
    void MoveTo(const CVector3 &, float);
    void MoveTo(const COORD3 &, float);
    void CleanUp();
    void BindLocationCollision(EALA::Character::CollisionData *);
    int SetAnimationState(const char *, int);
    int SetAnimationState(const char *, EALA::Task *);
    void GetTMLocalToWorld(EAGL::Transform &) const;
    void ApplyForceTo(const CVector3 &, float);
    void BeginHurtling();
    void SetLightVolumeTransitionDuration(float);
    void LineAlmostCollided(CBullet *) const; // An empty body returning nothing.
    void *GetSkeleton();
    void Shutdown();
    class StateDone;
    void SetMoveTarget(const CVector3 &);
    enum LocomotionType { LocomotionUnknown = 0 }; // Original name; its members are unknown.
    LocomotionType DetermineLocomotionType(float, LocomotionType);
    int IsVisible(CDrawContext &) const;
    void Draw(CDrawContext &);
    static void DrawMesh(CDrawContext &, void **);
    static int ClipSphere(const MATRIX4 &, const COORD3 &, float, float, float &);
    void UpdateLocalBoundingVolume(ISceneNode::EVolumeType, const CVector3 &, const CVector3 &);
    int AttachObject(CStaticObject *, char *, int);
    int DetachObject(char *);
    int GetAttachPointMatrix(char *, CMatrix *);
    void OnSwordDraw();
    void OnSwordSheath();
    void AttachFloatingBone();
    void DetachFloatingBone();
    float GetValue(const char *);
    int EvaluateScript(const char *, unsigned int, const char *);
    float FadeValue() const;
    void FadeOut(float);
    void FadeIn(float);
    void PreTransform(const CMatrix &);
    void Transform(const CMatrix &);
    void Rotate(const CVector3 &, float);
    void Pitch(float);
    void Roll(float);
    void SetBasis(const CVector3 &, const CVector3 &, const CVector3 &);
    struct ExtentNode {
        unsigned char unknown_0[8];
        float value;
    };
    unsigned char unknown_0[0x20];
    ExtentNode *sortMinX, *sortMaxX, *sortMinY, *sortMaxY; // +0x20
    unsigned char unknown_30[0x44 - 0x30];
    int field_44; // +0x44; cleared by Init.
    int field_48; // +0x48; cleared by Init.
    float scale;         // +0x4c; multiplies the animation's motion and bounds. Descriptive.
    int alwaysVisible;   // +0x50; IsVisible returns 1 while set. Descriptive.
    unsigned char choreographer[0x88 - 0x54]; // +0x54; EALA::Character::Choreographer methods run on it. Extent unknown.
    CMatrix *JointMatrices() const { return *(CMatrix **)(skeleton + 0x34); } // +0xbc
    unsigned char skeleton[0xc8 - 0x88]; // +0x88; GetSkeleton returns this address. Extent from the destructor.
    CharacterMeshView *mesh;      // +0xc8; an EALA::Resource::Pointer<Mesh>, released by the destructor.
    unsigned char unknown_cc[4];
    CMatrix transform;   // +0xd0
    IVolume *localVolumeType3; // +0x110
    IVolume *localVolumeType0; // +0x114
    IVolume *worldVolumeType3; // +0x118
    IVolume *worldVolumeType0; // +0x11c
    BSObject *script;    // +0x120
    int collisionId;     // +0x124
    CAnimatedVolume *animatedVolume; // +0x128
    unsigned char unknown_12c[0x130 - 0x12c];
    CVector3 moveTarget; // +0x130; SetMoveTarget stores it.
    unsigned char weakNode[0x154 + 4 - 0x140]; // +0x140; a WeakPtr<ISceneNode, 8>, from the table the constructor stores.
    float yaw;           // +0x158
    int animCallback;    // +0x15c
    int pendingCallback; // +0x160
    float callbackStart; // +0x164
    float callbackLast;  // +0x168
    unsigned char unknown_16c[0x170 - 0x16c];
    CVector3 adjustTarget; // +0x170
    float adjustValue;   // +0x180; descriptive.
    int adjustBlend;     // +0x184
    int locomotionType; // +0x188; DetermineLocomotionType's last result.
    const char *stateName; // +0x18c
    CVector3 previousPosition; // +0x190; soldier CommitUpdate copies the position here.
    CVector3 velocity;   // +0x1a0
    CVector3 animMove;   // +0x1b0
    float animRemaining; // +0x1c0
    float animStep;      // +0x1c4
    float animRate;      // +0x1c8
    float animMoveScale; // +0x1cc
    float yawCopy;       // +0x1d0; SetYaw keeps it equal to yaw.
    float yawRate;       // +0x1d4; cleared by Halt. Descriptive.
    unsigned char unknown_1d8[0x1e0 - 0x1d8];
    CVector3 committedOffset; // +0x1e0
    CQuaternion committedRotation; // +0x1f0
    CVector3 pendingOffset; // +0x200
    CQuaternion pendingRotation; // +0x210
    int hurtling;        // +0x220; meaning inferred.
    int fallStarted;     // +0x224; set by BeginUpdate while gravity applies. Descriptive.
    int hurtlingState;   // +0x228; meaning inferred.
    int animTurned;      // +0x22c; cleared by UpdateAnimation. Descriptive.
    unsigned char unknown_230[0x234 - 0x230];
    int allowAnimationTranslation; // +0x234; set by BIFunc_SetAllowAnimationTranslation.
    int attached;        // +0x238
    int attachedToLocation; // +0x23c
    float attachBlend;   // +0x240
    unsigned char unknown_244[0x250 - 0x244];
    CMatrix attachLocation; // +0x250
    int moveTargetPending; // +0x290; set by SetMoveTarget, cleared by the path callbacks.
    int gravity;    // +0x294
    int groundCheck; // +0x298
    int animMoved;   // +0x29c
    int hurtlingBlocked; // +0x2a0; BeginHurtling does nothing while set. Descriptive.
    CLightVolumeManager lightVolumes; // +0x2a4
    short field_2c0; // +0x2c0; Init stores 40.
    int stateTask;       // +0x2c4; a CAnimObject::StateDone, passed to SetState by address: table, event, owner.
    int stateValue;      // +0x2c8
    CAnimObject *stateOwner; // +0x2cc
    class AO_OpcodeHandler : public EALA::Character::Opcode::Handler {
      public:
        virtual int Execute(EALA::Character::Opcode &);
        CAnimObject *owner; // +4
    } opcodeHandler; // +0x2d0; class name from _vt.Q211CAnimObject16AO_OpcodeHandler.
    int poseEvaluated;   // +0x2d8; descriptive.
    int stateDone; // +0x2dc; set by Init, the constructor and StateDone::Execute. Descriptive.
    void SetAttached(bool);
    void StartAnimAdjust(const CVector3 &, float, bool);
};

class CAISoldierObject;
class CSoldierObject {
  public:
    static void InitClass();
    // A short list reset by clearing the count at +8; the name is descriptive.
    struct VolumeList {
        unsigned char unknown_0[8];
        int count;
        unsigned char unknown_c[0x18 - 0xc];
        void Clear() { count = 0; }
    };
    void InitAI();
    void InitAI2();
    CAIObject *GetAIObject();
    void AttemptUpdate(float);
    void CommitUpdate();
    void UpdateLocalBoundingVolume(ISceneNode::EVolumeType, const CVector3 &, const CVector3 &);
    IVolume *GetLocalBoundingVolume(ISceneNode::EVolumeType) const;
    IVolume *GetWorldBoundingVolume(ISceneNode::EVolumeType) const;
    void SetAttachedLight(CLight *, const CVector3 &);
    void TranslateFromScript(const CVector3 &, const CVector3 &, EBSEventEnum);
    int AddWeapon(int, int, int);
    int AttachObject(int, char *, int);
    int AttachObject(CStaticObject *, char *, int);
    int AttachMesh(CStaticMesh *, char *, int);
    void SetBulletEmitter(CStaticObject *, char *);
    void OnSwordDraw();
    void OnSwordSheath();
    void GetLookMatrix(CMatrix &);
    void CleanUp();
    CStaticObject *DetachObject(char *);
    CStaticMesh *DetachMesh(char *);
    void OnCollision(const CCollision &);
    void HandleBulletCollision(CBullet *, const CCollision &);
    int EjectHelmet(CBullet *, const CCollision &);
    void GetBulletEmitterMatrix(CMatrix &);
    int GetAttachPoint(const char *);
    int GetAttachPointMatrix(char *, CMatrix *);
    void FadeOut(float);
    float FadeValue() const;
    void EnableAimModifier(bool);
    float GetLipsynchBlendWeight();
    void SetCurrentGag(int, int);
    void OnLocomotionEnd();
    void Destroy();
    CAIDoodad *GetAIDoodad();
    const CAIDoodad *GetAIDoodad() const;
    CSoldierObject *AsSoldierObject();
    const CSoldierObject *AsSoldierObject() const;
    void Draw(CDrawContext &);
    void RemoveWeapons();
    // Each client holds its soldier at +4; only the symbols' methods are declared.
    class AimModifierClient {
      public:
        void GetTMLocalToWorld(EAGL::Transform &) const;
        unsigned char unknown_0[4];
        CSoldierObject *soldier; // +4
    };
    class AimClient {
      public:
        void GetTarget(COORD3 &) const;
        unsigned char unknown_0[4];
        CSoldierObject *soldier; // +4
    };
    class LookAtClient {
      public:
        void GetTarget(COORD3 &) const;
        unsigned char unknown_0[4];
        CSoldierObject *soldier; // +4
    };
    // Original nested enum tag; members are descriptive.
    enum Partition { PartitionNone = 0, Partition1 = 1, Partition2 = 2, Partition3 = 3 };
    unsigned char unknown_0[0x120];
    BSObject *scriptObject; // +0x120
    unsigned char unknown_124[0x1a8 - 0x124];
    float fallSpeed;        // +0x1a8; descriptive: OnCollision only counts a ground check once it is not positive.
    unsigned char unknown_1ac[0x220 - 0x1ac];
    int groundCheckPending; // +0x220
    unsigned char unknown_224[4];
    int groundCheckState;   // +0x228; cleared before the ground-check event.
    unsigned char unknown_22c[0x2c0 - 0x22c];
    short groundSound;      // +0x2c0; descriptive: the surface's sound index, or 40 without a surface.
    unsigned char unknown_2c2[0x2e0 - 0x2c2];
    unsigned char aiDoodad[0x2ec - 0x2e0]; // +0x2e0; a CAISoldierDoodad.
    int locomotionInfo;     // +0x2ec; returned by address.
    unsigned char unknown_2f0[0x334 - 0x2f0];
    int nonVisDestructDisabled; // +0x334; the inverse of BIFunc_EnableNonVisDestruct's flag.
    unsigned char unknown_338[0x340 - 0x338];
    int clsnConstraints;    // +0x340; start of a CClsnConstraintsBase of unestablished extent.
    VolumeList volumeLists[3]; // +0x344
    unsigned char unknown_38c[0xb10 - 0x38c];
    int collidedThisFrame;  // +0xb10; descriptive.
    int canBePushed;        // +0xb14
    unsigned char unknown_b18[8];
    CMatrix defaultEmitterMatrix; // +0xb20
    CVector3 upperTranslation; // +0xb60; set by SetUpperTranslation.
    float upperXAngleMin;   // +0xb70; the second argument of SetUpperXAngleDifference goes to +0xb74; descriptive.
    float upperXAngleMax;   // +0xb74
    float upperZAngleMin;   // +0xb78; descriptive.
    float upperZAngleMax;   // +0xb7c
    float moveBlend;        // +0xb80; descriptive.
    CWeapon *weapons[1];    // +0xb84; slot table, indexed by slot number (extent unknown).
    unsigned char unknown_b88[0xba0 - 0xb88];
    // Eight 112-byte attach slots, up to the next member; field names are descriptive.
    struct AttachSlot {
        CStaticObject *object;
        const CMatrix *matrix;
        unsigned char unknown_8[8];
        CMatrix inverse; // +0x10
        const char *name; // +0x50; set by GetAttachPoint.
        unsigned char unknown_54[4];
        int active;      // +0x58
        int keepObject;  // +0x5c
        int hasInverse;  // +0x60
        int addToScene;  // +0x64
        int owned;       // +0x68
        unsigned char unknown_6c[4];
        // Above word alignment, inferred: only then do indexed members compile as
        // (this + i * 112) + offset, as they do in the original.
    } __attribute__((aligned(8))) attachSlots[8]; // +0xba0
    int currentWeaponSlot;  // +0xf20
    int emitterActive;      // +0xf24
    int emitterSlot;        // +0xf28
    unsigned char unknown_f2c[4];
    CMatrix emitterMatrix;  // +0xf30
    unsigned char unknown_f70[0xf7c - 0xf70];
    // Facial-expression blend state written by BIFunc_SetExpression; names are descriptive.
    struct ExpressionState {
        int current;        // +0xf7c
        int pending;        // +0xf80
        float currentBlend; // +0xf84
        float pendingBlend; // +0xf88
        float currentTime;  // +0xf8c
        float pendingTime;  // +0xf90
        float currentRate;  // +0xf94
        float pendingRate;  // +0xf98
    } expression;
    // Blink timing written by BIFunc_SetBlinkState; names are descriptive.
    float blinkTimer;       // +0xf9c
    float blinkMinInterval; // +0xfa0
    float blinkMaxInterval; // +0xfa4
    float blinkCloseTime;   // +0xfa8
    float blinkHoldTime;    // +0xfac
    float blinkOpenTime;    // +0xfb0
    unsigned char unknown_fb4[4];
    float blinkPhaseEnd[3]; // +0xfb8; running totals of the open, close and hold times.
    int faceAnimationPending; // +0xfc4
    unsigned short faceAnimationState; // +0xfc8
    unsigned char unknown_fca[0xfd0 - 0xfca];
    unsigned char localVolumeType1[0xff0 - 0xfd0]; // +0xfd0; a volume returned by address.
    unsigned char worldVolumeType1[0x1010 - 0xff0]; // +0xff0
    unsigned char localVolumeType2[0x1030 - 0x1010]; // +0x1010
    unsigned char worldVolumeType2[0x1050 - 0x1030]; // +0x1030
    int voiceHandle;        // +0x1050; -12 when no voice is playing.
    unsigned char unknown_1054[0x1058 - 0x1054];
    int helmetChance;       // +0x1058; a percentage. Name from Frontline.
    float helmetDamageScale; // +0x105c; name from Frontline.
    unsigned char fadeStorage[0x1070 - 0x1060]; // +0x1060; a CObjectFadeManager. Extent bounded.
    int weaponsRemoved;     // +0x1070; descriptive.
    int headTrackPlayer;    // +0x1074
    unsigned char unknown_1078[0x1080 - 0x1078];
    unsigned char shadowStorage[0x10a8 - 0x1080]; // +0x1080; CCharacterShadow methods run on it. Extent bounded.
    CJawController jaw;     // +0x10a8
    unsigned char unknown_10e8[0x10f4 - 0x10e8];
    CWeapon *otherWeapon;   // +0x10f4; destroyed apart from the slot table.
    float hitTimer;         // +0x10f8; name from Frontline.
    int hitCount;           // +0x10fc; name from Frontline.
    signed char headTransState; // +0x1100
    unsigned char unknown_1101[0x1138 - 0x1101];
    unsigned char aimModifier[0x1148 - 0x1138]; // +0x1138; EALA::Character::AimModifier methods run on it.
    int aimSuspended;       // +0x1148; set by BIFunc_SuspendAiming.
    unsigned char unknown_114c[0x12a4 - 0x114c];
    unsigned char propAimModifier[0x12b4 - 0x12a4]; // +0x12a4
    int propAimSuspended;   // +0x12b4; set by BIFunc_SuspendPropAiming.
    unsigned char unknown_12b8[0x1410 - 0x12b8];
    unsigned char headTrackModifier[0x157c - 0x1410]; // +0x1410; first bytes an AimModifier. Extent bounded.
    int turnAnglePending;   // +0x157c
    float turnAngle;        // +0x1580
    int savedGagState[3];   // +0x1584; filled by SaveGagState from the choreographer client.
    float savedGagTime;     // +0x1590
    int currentGag;         // +0x1594
    int currentGagType;     // +0x1598
    float lipSyncSum;       // +0x159c; absolute sample values since the last update.
    float lipSyncSamples;   // +0x15a0; samples since the last update.
    int flagTeam;           // +0x15a4; -1 when the soldier carries no flag.
    CAISoldierObject *aiObject; // +0x15a8
    // Two state-done records like CAnimObject's stateTask/stateValue/stateOwner: one for the
    // lower partition and one for the upper.
    struct PartitionState {
        int task;     // passed to SetState by address
        int value;
        CSoldierObject *owner;
    };
    PartitionState lowerState; // +0x15ac
    PartitionState upperState; // +0x15b8
    unsigned char lipsyncMutex[0x15e0 - 0x15c4]; // +0x15c4; passed to MUTEX_lock.
    float lipsyncBlendWeight; // +0x15e0
    unsigned char unknown_15e4[0x15e8 - 0x15e4];
    int japanese;           // +0x15e8; set by BIFunc_SetJapanese.
    unsigned char lipSyncFilterDef[0x1c54 - 0x15ec]; // +0x15ec; returned by address. Extent bounded.
    float reloadTimer;      // +0x1c54; reset to -1 by BIFunc_BotDoneReloading.
    void SetHeadTrans(bool);
    CWeapon *GetCurrentWeapon() const;
    CWeapon *GetWeapon(int) const;
    int GetCurrentWeaponType() const;
    bool IsCurrentWeaponReloading() const;
    void SetExpression(EExpressionType, float);
    void SetBlink(int);
    void SetUpperTranslation(const CVector3 &);
    void SetUpperXAngleDifference(float, float);
    void SetUpperZAngleDifference(float, float);
    int SetFaceAnimationState(unsigned short, int);
    static unsigned char m_partitionLower[0x30], m_partitionUpper[0x30];
    void SetLipsynchBlendWeight(float);
    int ShouldDrawFlag(CDrawContext &);
    void *GetLipSyncFilterDef();
    static int LipSyncFilterUpdate(void *, int, void *, void *, int);
    static void LipSyncFilterInit(void *, int, int);
    static void LipSyncFilterRestore(void *);
    int EvaluateScript(const char *, unsigned int, const char *);
    void UpdateCollisionVolumes();
    int Constrain(float);
    void UpdateAnimation(float);
    int SetAnimationState(const char *, int);
    int SetAnimationState(const char *, EALA::Task *);
    void SetHeadTrackPlayer(bool);
    void EnableSkeletalPartition(Partition);
    void DisableSkeletalPartition(Partition);
    int SetAnimationState(const char *, int, Partition);
    ISceneNode *GetAttachedObjectAt(char *);
    void ResumeAimModifier();
    void ResumePropAimModifier();
    void EnablePropAimModifier(bool);
    void SaveGagState();
    void FireMMG(CStaticObject *);
    void SpecifyTurnAngle(float);
    void SetCurrentWeapon(int, int, bool);
    void EjectHelmet(CVector3 &);
    int IsInAnyOtherCompartment(int);
    void FireCurrentWeapon(float, EFireMethod);
    int FireWeapon(int, const CVector3 &, const CVector3 &, float, bool);
};

// Method-only view of a hierarchy object.
// Inline in the original: the soldier's current weapon slot as a scene node.
inline ISceneNode *CurrentWeaponNode(CSoldierObject *soldier) { return (ISceneNode *)soldier->weapons[soldier->currentWeaponSlot]; }

class IVolume;
// The direct calls to CStaticObject::Draw and CAttachableObject::UpdateAttachedObjects on the
// same pointer show both views start at the object's first byte; the attachable base's own
// storage is not declared here.
class CHierObject : public CStaticObject {
  public:
    IVolume *GetSubVolume(int) const;
    void RegisterBulletDamage(int, int, int, float);
    float GetBulletDamageMultiplierByClass(int, int);
    CStaticObject *GetSubObject(int);
    void SwapOut(CStaticMesh *, MotionData *);
    void ModifyMotionData(MotionData *);
    void Init(CHierObject *, int, int, int, CVector3 &);
    void OnCollision(const CCollision &);
    void CommitUpdate();
    void CommitScriptMotionUpdate();
    void CommitAnimMotionUpdate();
    void BeginScriptMotionUpdate(float);
    void CollisionResetMovement();
    void SetRotation(CQuaternion &);
    void GetRotation(CQuaternion &);
    void GetAttachPointMatrix(int, CMatrix &);
    void AddObjectToTree(CHierObject *, int, int);
    int RemoveObjectFromTree(CHierObject *, int);
    void CleanUp();
    void AddHierChild(CHierObject *);
    void BeginUpdate(float);
    void DoBeginUpdate(float, bool);
    CVector3 GetWorldLinearVelocity() const;
    void AttemptUpdate(float);
    void AttemptUpdateFromMotion(float);
    void AttemptScriptMotionUpdate(float);
    void AttemptAnimMotionUpdate(float);
    void BeginAnimMotionUpdate(float);
    void AttemptUpdateOfAttachedObjects();
    int SetHierCollisionState(int);
    IVolume *GetWorldBoundingVolume(ISceneNode::EVolumeType) const;
    void SetInheritedMatrix(CMatrix &);
    void SetAttachPointMatrix(CMatrix &);
    int IsVisible(CDrawContext &) const;
    void Draw(CDrawContext &);
    BPDLightVolume *GetLightVolume();
    void StartMotionPlayback(int, int, EBSEventEnum);
    void SerializeSaveData();
    int GetObjectType() const;
    CHierObject *AsHierObject();
    const CHierObject *AsHierObject() const;
    int SetAnimationState(const char *, int);
    struct HierFlags {
        unsigned int disabled : 1;   // Bit 31; BeginUpdate does nothing while set. Descriptive.
        unsigned int mountedGun : 1; // Bit 30; positions the mounted-gun player. Descriptive.
        unsigned int root : 1;       // Bit 29; root of a hierarchy, owns a CSG volume. Descriptive.
        unsigned int collided : 1;   // Bit 28; set by SetHierCollisionState.
        unsigned int unknown_low : 28;
    };
    struct BulletDamage {
        int minClass, maxClass;
        float multiplier;
    };
    unsigned char unknown_2f0[0x980 - sizeof(CStaticObject)];
    CHierObject *firstChild;  // +0x980
    CHierObject *nextSibling; // +0x984
    HierFlags hierFlags;      // +0x988
    int treeIndex;            // +0x98c
    CHierObject *root;        // +0x990
    unsigned char csgVolume[0x9a0 - 0x994]; // +0x994; a CCSGVolume on root objects. Extent as declared.
    CHierObject *subObjects[20]; // +0x9a0; the root's table of the tree by index.
    CMatrix inheritedMatrix;  // +0x9f0
    CMatrix attachPointMatrix; // +0xa30
    CVector3 attachOffset;    // +0xa70
    CVector3 attachDelta;     // +0xa80; added to attachOffset on commit.
    // +0xa90: the animated objects touching this one, collected from contacts.
    struct AttachedList {
        ISceneNode **items;
        unsigned char unknown_4[4];
        int count;
        int capacity;
        void Add(ISceneNode *node) {
            if (count < capacity)
                items[count++] = node;
        }
    };
    AttachedList touching;
    unsigned char unknown_aa0[0xb28 - 0xaa0];
    BulletDamage bulletDamage[2]; // +0xb28; class ranges, unused entries hold -1.
};

// Forwards to an owned animated object; the pointer is at +0xb40. Original class, member-only view.
class CHierAnimObject : public CHierObject {
  public:
    class StateDone;
    void Init(CHierObject *, int, int, int, CVector3 &, CAnimObject *);
    int AttachAnimatedObject();
    int SetAnimationState(const char *, int);
    int FindAttachNode(int, CMatrix *);
    CAnimObject *AsAnimObject();
    const CAnimObject *AsAnimObject() const;
    void CleanUp();
    void Draw(CDrawContext &);
    IVolume *GetWorldBoundingVolume(ISceneNode::EVolumeType) const;
    void AttemptUpdate(float);
    void CommitUpdate();
    void OnCollision(const CCollision &);
    CVector3 GetWorldLinearVelocity() const;
    CAnimObject *animObject; // +0xb40
    unsigned char unknown_b44[0xb50 - 0xb44];
    CMatrix localTransform; // +0xb50
    unsigned char doneState[12]; // +0xb90: a StateDone.
};

class TriggerObject;
int AssetCRC(char *);
int *CreateRootHierObject(TriggerObject *, int, int, int);
void CreateSubHierObject(CHierObject *, int, int, unsigned int, int, int);
void CreateSubHierAnimObject(CHierObject *, int, int, unsigned int, int, int, int);
int *CreateObject(TriggerObject *, int, int, int, void *);

// Scoped view of a bounding volume; only the centre at +0x40 is read.
struct BoundingVolumeView {
    unsigned char unknown_0[0x40];
    CVector3 center;
};
// Static creators; the returned nodes are used through the scene-node tables.
class CProjector {
  public:
    static IMovingSceneNode *Create(unsigned int, bool);
};
class CGlare {
  public:
    static IMovingSceneNode *Create(int, bool);
};

#include "AnimShape.h"

#include "DecalManager.h"

#include "Color.h"

// Static screen-fade entry points.
class CFader {
  public:
    static void Flash(CColor, float, float);
    static void FadeIn(CColor, float);
    static void FadeOut(CColor, float);
    static CColor BLACK;
};

// Static motion-blur control.
namespace MotionBlur {
void SetFrameBlur(float, float, float, float);
void StopFrameBlur();
void Update(float);
void Pause();
extern float gPrevAmount;
extern float gCurrentAmount;
extern float gTransitionTimer;
extern float gDeltaAmount;
extern float gAmount;
extern float gRampIn;
extern float gRampOut;
extern float gDuration;
extern int gState;
extern int gPaused;
}

// Virtual-interface view of the animated-object tables (slots 89-113 after the
// IMovingSceneNode slots), in the order of the original CSoldierObject table.
// Names come from the symbols in each slot; return and parameter types beyond
// the symbols are placeholders. It declares no storage: callers convert the
// scene-node pointers they already hold, which point at the same object.
namespace EALA { class Task; }
namespace EAGL { class Transform; }
class CAnimObjectInterface : public IMovingSceneNode {
  public:
    virtual void Init();
    virtual void Shutdown();
    virtual void CleanUp();
    virtual int SetAnimationState(const char *, int);
    virtual int SetAnimationState(const char *, EALA::Task *);
    virtual void UpdateAnimation(float);
    virtual void SetMoveTarget(const CVector3 &);
    virtual void SetScript(BSObject *);
    virtual void UpdateLocalBoundingVolume(ISceneNode::EVolumeType, const CVector3 &, const CVector3 &);
    virtual int LineAlmostCollided(CBullet *) const;
    virtual void GetTMLocalToWorld(EAGL::Transform &) const;
    virtual int AttachObject(CStaticObject *, char *, int);
    virtual int DetachObject(char *);
    virtual int GetAttachPointMatrix(char *, CMatrix *);
    virtual void OnSwordDraw();
    virtual void OnSwordSheath();
    virtual void AttachFloatingBone();
    virtual void DetachFloatingBone();
    virtual void SetAnimCallback(EAnimCallback);
    virtual float FadeValue() const;
    virtual void FadeOut(float);
    virtual void FadeIn(float);
    virtual int AttachObject(int, char *, int);
    virtual void GetAimTarget(CVector3 &) const;
    virtual void EnableAimModifier(bool);
};

// Virtual-interface view of the static/hierarchy-object tables (slots 89-109
// after the IMovingSceneNode slots), in the order of the original CHierObject
// table. Names come from the symbols in each slot; types beyond the symbols are
// placeholders. Callers convert the scene-node pointers they already hold.
class CPlayerObject;
class CAnimObject;
class CDrawContext;
class CAttachableObject {
  public:
    unsigned char unknown_0[0x4c];
    // The static mesh record is reached through a pointer at +0x4c; +0x30 holds the
    // attach-node count and +0x50 the first 80-byte node, with its id at +0x40.
    struct AttachMesh {
        struct Node {
            CMatrix matrix;
            int id;
            unsigned char unknown_44[12];
        };
        unsigned char unknown_0[0x30];
        int nodeCount;
        unsigned char unknown_34[0x50 - 0x34];
        Node nodes[1];
    } **mesh;
    unsigned char unknown_50[0x300 - 0x50];
    ISceneNode *children[20]; // +0x300
    CMatrix childMatrices[20];     // +0x350
    int childTypes[20];            // +0x850; 1 static, 2 animated, 3 player.
    unsigned int childQueueIdentity[20]; // +0x8a0
    int childNodes[20];            // +0x8f0
    int GetFreeAttachSlot();
    void Draw(CDrawContext &);
    int IsVisible(CDrawContext &) const;
    void BeginUpdate(float);
    void UpdateAI(float);
    void CommitAI();
    void AttemptUpdate(float);
    void CommitUpdate();
    int FindAttachNode(int, CMatrix *);
    int GetObjectType() const;
    void UpdateAttachedObjects();
    int AttachObject(CPlayerObject *, int, bool);
    int AttachObject(CAnimObject *, int);
    int AttachObject(CStaticObject *, int);
    int DetachObject(ISceneNode *);
    void DetachAllObjects();
    void UpdateSingleAttachedObject(int);
    int AttachAndAlignObject(CAnimObject *, int);
    int GetFirstChildHandle();
    int GetNextChildHandle(int);
    ISceneNode *GetChild(int);
};
class CAITankObject;
class CAIDoodad;
class CTankObject : public CHierObject {
  public:
    void CommonInit();
    void InitAI();
    void CleanUp();
    void BeginUpdate(float);
    void UpdateAI(float);
    void GetLookMatrix(CMatrix &);
    void HandleBulletCollision(CBullet *, const CCollision &);
    void AttemptUpdate(float);
    void CommitUpdate();
    int AddWeapon(int, int);
    CAIDoodad *GetAIDoodad();
    const CAIDoodad *GetAIDoodad() const;
    CAIObject *GetAIObject();
    CTankObject *AsHierTankObject();
    void FireSubObject(int);
    void MapWeaponSlot(int, int);
    unsigned char aiDoodad[0xb4c - 0xb40]; // +0xb40; a CAITankDoodad.
    CAITankObject *aiObject; // +0xb4c
    int lookObject;          // +0xb50; set by BIFunc_HierObjectSetLookObject.
    int pathNodeTimer;       // +0xb54; counts down to the next path-node refresh.
    int weaponSlots[8];      // +0xb58; extent bounded by the next member.
    CWeapon *weapons[3];     // +0xb78; loops in CleanUp and BeginUpdate cover three.
};
class CStaticObjectInterface : public IMovingSceneNode {
  public:
    virtual void Init();
    virtual void SetScript(BSObject *);
    virtual int GetObjectType() const;
    virtual void TranslateFromScript(const CVector3 &, const CVector3 &, EBSEventEnum);
    virtual void RotateFromScript(const CVector3 &, const CVector3 &, EBSEventEnum);
    virtual void ScaleFromScript(float, float, EBSEventEnum);
    virtual void StartMotionPlayback(int, int, EBSEventEnum);
    virtual void *AsThrownObject();
    virtual void *AsWeaponObject();
    virtual CTankObject *AsHierTankObject();
    virtual void AttemptScriptMotionUpdate(float);
    virtual void AttemptAnimMotionUpdate(float);
    virtual void AttemptPathMotionUpdate(float);
    virtual void CommitScriptMotionUpdate();
    virtual void CommitAnimMotionUpdate();
    virtual void CommitPathMotionUpdate();
    virtual int FindAttachNode(int, CMatrix *);
    virtual void CleanUp();
    virtual int SetAnimationState(const char *, int);
    virtual void BeginScriptMotionUpdate(float);
    virtual void BeginAnimMotionUpdate(float);
};

#endif
