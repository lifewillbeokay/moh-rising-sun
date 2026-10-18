// AI-assisted scoped reconstruction from GR8E69; see docs/Script.md.
#ifndef GAME_SCENE_OBJECTS_H
#define GAME_SCENE_OBJECTS_H
#include "SceneNode.h"
#include "AITargeting.h"
struct BSObject;
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

// Member-only views of scene-object classes reached through the ISceneNode
// casts. Each view starts at the ISceneNode subobject (the casts return it
// unadjusted) and names only the observed members; bases, virtual interfaces
// and complete sizes are not declared. Built-ins that need an inherited virtual
// convert the view back to ISceneNode without adjustment.
// Method-only view of the soldier's embedded jaw controller.
enum EJawType { JawTypeUnknown = 0 };
class CJawController {
  public:
    void SetMotionType(EJawType);
};

struct MotionData;
MotionData *InitMotionData(const char *);
class CStaticObject {
  public:
    void StopMotionPlayback();
    void ModifyMotionData(MotionData *);
    void SetMovementPath(int, bool, bool, float, float, float, float, BSObject *, float, bool);
    void ResumeMotionPlayback();
    void PlayAnimation(int, int, int, EBSEventEnum);
    void FadeIn(float);
    void FadeOut(float);
    struct Flags {
        unsigned int unknown_31_25 : 7;
        unsigned int allowCollisionDamage : 1; // Bit 24.
        unsigned int pathFlag1 : 1;            // Bit 23: BIFunc_MechSetPathFlag selector 1.
        unsigned int unknown_low : 23;
    };
    unsigned char unknown_0[0x110];
    float pathScale;      // +0x110; BIFunc_MechSetPathFlag stores value / 100.
    unsigned char unknown_114[0x214 - 0x114];
    int maxLightPriority; // +0x214
    Flags flags;          // +0x218
    unsigned int mechFlags; // +0x21c; bits set and cleared by BIFunc_MechSetFlag.
};

class CAnimObject {
  public:
    unsigned char unknown_0[0x234];
    int allowAnimationTranslation; // +0x234; set by BIFunc_SetAllowAnimationTranslation.
    unsigned char unknown_238[0x294 - 0x238];
    int gravity;    // +0x294
    int groundCheck; // +0x298
    void SetAttached(bool);
    void StartAnimAdjust(const CVector3 &, float, bool);
};

class CSoldierObject {
  public:
    // Original nested enum tag; members are descriptive.
    enum Partition { PartitionNone = 0, Partition1 = 1, Partition2 = 2, Partition3 = 3 };
    unsigned char unknown_0[0x120];
    BSObject *scriptObject; // +0x120
    unsigned char unknown_124[0x220 - 0x124];
    int groundCheckPending; // +0x220
    unsigned char unknown_224[4];
    int groundCheckState;   // +0x228; cleared before the ground-check event.
    unsigned char unknown_22c[0x2ec - 0x22c];
    int locomotionInfo;     // +0x2ec; returned by address.
    unsigned char unknown_2f0[0x334 - 0x2f0];
    int nonVisDestructDisabled; // +0x334; the inverse of BIFunc_EnableNonVisDestruct's flag.
    unsigned char unknown_338[0xb14 - 0x338];
    int canBePushed;        // +0xb14
    unsigned char unknown_b18[0xb84 - 0xb18];
    CWeapon *weapons[1];    // +0xb84; slot table, indexed by slot number (extent unknown).
    unsigned char unknown_b88[0xf20 - 0xb88];
    int currentWeaponSlot;  // +0xf20
    unsigned char unknown_f24[0xf7c - 0xf24];
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
    unsigned char unknown_fb4[0x1050 - 0xfb4];
    int voiceHandle;        // +0x1050; -12 when no voice is playing.
    unsigned char unknown_1054[0x10a8 - 0x1054];
    CJawController jaw;     // +0x10a8
    unsigned char unknown_10a9[0x1148 - 0x10a9];
    int aimSuspended;       // +0x1148; set by BIFunc_SuspendAiming.
    unsigned char unknown_114c[0x12b4 - 0x114c];
    int propAimSuspended;   // +0x12b4; set by BIFunc_SuspendPropAiming.
    unsigned char unknown_12b8[0x15e8 - 0x12b8];
    int japanese;           // +0x15e8; set by BIFunc_SetJapanese.
    unsigned char unknown_15ec[0x1c54 - 0x15ec];
    float reloadTimer;      // +0x1c54; reset to -1 by BIFunc_BotDoneReloading.
    void SetHeadTrans(bool);
    void SetFaceAnimationState(unsigned short, int);
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
};

// Method-only view of a hierarchy object.
// Inline in the original: the soldier's current weapon slot as a scene node.
inline ISceneNode *CurrentWeaponNode(CSoldierObject *soldier) { return (ISceneNode *)soldier->weapons[soldier->currentWeaponSlot]; }

class CHierObject {
  public:
    void RegisterBulletDamage(int, int, int, float);
    float GetBulletDamageMultiplierByClass(int, int);
    CStaticObject *GetSubObject(int);
    void ModifyMotionData(MotionData *);
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

// Static animated-texture operations keyed by texture CRC.
class AnimShape {
  public:
    // Original nested enum tag; the observed values are ASCII state letters.
    enum EState { StateDefault = '-', StateL = 'L', Statel = 'l', StateO = 'O', Stateo = 'o', StateP = 'P', Statep = 'p' };
    static void SetStateByCRC(int, EState);
    static void InitByCRC(int, EState, float);
    static void ResetByCRC(int);
    static void AdvanceByCRC(int);
    static void SetFrameByCRC(int, float);
    static void SetFrameRateByCRC(int, float);
};

// Static decal operations used by the script built-ins.
class CDecalManager {
  public:
    static void ClearDecals();
};

// Four-byte colour passed by value; channel names are descriptive.
class CColor {
  public:
    unsigned char r, g, b, a;
    CColor() {}
    CColor(unsigned char ar, unsigned char ag, unsigned char ab, unsigned char aa) : r(ar), g(ag), b(ab), a(aa) {}
};

// Static screen-fade entry points.
class CFader {
  public:
    static void Flash(CColor, float, float);
    static void FadeIn(CColor, float);
    static void FadeOut(CColor, float);
};

// Static motion-blur control.
class MotionBlur {
  public:
    static void SetFrameBlur(float, float, float, float);
};

// Virtual-interface view of the animated-object tables (slots 89-113 after the
// IMovingSceneNode slots), in the order of the original CSoldierObject table.
// Names come from the symbols in each slot; return and parameter types beyond
// the symbols are placeholders. It declares no storage: callers convert the
// scene-node pointers they already hold, which point at the same object.
namespace EALA { class Task; }
namespace EAGL { class Transform; }
enum EAnimCallback { AnimCallbackUnknown = 0 };
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
class CAttachableObject {
  public:
    int AttachObject(CPlayerObject *, int, bool);
    int AttachObject(CAnimObject *, int);
    int AttachObject(CStaticObject *, int);
    int DetachObject(ISceneNode *);
    void DetachAllObjects();
    int AttachAndAlignObject(CAnimObject *, int);
    int GetFirstChildHandle();
    int GetNextChildHandle(int);
    ISceneNode *GetChild(int);
};
class CTankObject {
  public:
    unsigned char unknown_0[0xb50];
    int lookObject; // +0xb50; set by BIFunc_HierObjectSetLookObject.
    void FireSubObject(int);
    void MapWeaponSlot(int, int);
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
