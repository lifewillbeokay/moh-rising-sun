// AI-assisted reconstruction from GR8E69; see docs/Targeting.md.
#ifndef GAME_AI_TARGETING_H
#define GAME_AI_TARGETING_H
#include "AIMovement.h"
#include "AISplinePath.h"
// Original enum tag; numeric members are descriptive, not recovered names.
enum AIFILTER_WEAPON_AIM_MODE {
    WeaponAimModeUnset = 0, WeaponAimMode1 = 1, WeaponAimMode2 = 2,
    WeaponAimMode6 = 6, WeaponAimMode8 = 8
};
class CAIObjectParameters;
// Original enum tag; its members are unknown.
enum ETeam { TeamUnknown = 0 };
class ISceneNode;
class CAIObject;
struct aistatus_match;
// Twelve-byte value copied by the original targeting callers. Field names and
// inline helper names are descriptive; method symbols retain original names.
class CAITarget {
public:
    int nonAI; // Four-byte selector: setters write zero for AI and one for script.
    CAIObject *aiObject;
    BSObject *scriptObject;
    CAITarget();
    CAITarget(CAIObject *);
    bool Matches(const CAITarget &);
    BSObject *GetScriptObject() const;
    void SetAsNonAITarget(BSObject *);
    void Nullify();
    void SetAsAITarget(CAIObject *);
    CVector3 GetPosition() const;
    CVector3 GetForward() const;
    bool HasTarget() const { return aiObject || scriptObject; }
};
typedef char CAITargetStorageSizeCheck[sizeof(CAITarget) == 12 ? 1 : -1];

// Observed prefix through the weapon aim mode at +0x104. This view
// does not establish the complete class allocation or virtual interface.
class CAITargeting {
public:
    unsigned char unknown00[4];
    CAIPhysics *physics;
    CAIObjectParameters *parameters;
    ISceneNode *sceneNode;
    CAITarget target;
    unsigned char unknown1c[4];
    CVector3 lastSeenPosition;
    float lastSeenTime;
    int lastSeenSet, visionVisible;
    unsigned char unknown3c[4];
    CVector3 targetPosition; // +0x40; scene query output.
    unsigned char unknown50[0x20];
    CVector3 modifiedPosition;
    unsigned char unknown80[0x74];
    int alerted; // +0xf4; set to 1 by BIFunc_AISendFlag flag 1.
    float targetUpdateTime; // +0xf8; written after the script-target update.
    int blindFireMode; // +0xfc; observed selectors 0, 1 and 2.
    int weapon; // +0x100; passed to AdjustAimPosition.
    AIFILTER_WEAPON_AIM_MODE aimMode; // +0x104.
    CAITarget GetTarget() const { return target; }
    void GetLastSeenTargetPosition(CVector3 &);
    void GetModifiedTargetPosition(CVector3 &);
    void SetLastSeenTargetPosition(const CVector3 &);
    int IsLastSeenTargetPositionSet();
    int CouldVisionSeeTarget();
    void Update();
    bool UpdateReactionTime();
    void UpdateTargetPosition();
    void UpdateNonAITargetPosition();
    void SetNonAITarget(BSObject *);
    void SetTarget(CAIObject *);
    void AdjustAimPosition(CVector3 *, int, AIFILTER_WEAPON_AIM_MODE *);
    void ChooseSniperAimMode(AIFILTER_WEAPON_AIM_MODE *);
    void ChooseBazookaAimMode(AIFILTER_WEAPON_AIM_MODE *);
    void GetAdjustedAimTargetPosition(CVector3 &);
    bool DoBlindFire();
    float GetDistanceToTargetSquaredXYZReal();
    BSObject *GetTargetScript() const;
    static void RegisterLocationTarget(int, bool, ETeam, BSObject *);
    static void DeRegisterLocationTarget(int);
    static CAITarget GetLocationTarget(int);
    bool IsNonAITarget() const;
    bool HasValidTarget() const;
    bool GetTargetGuessPosition(CVector3 &);
};
typedef char AITargetingPrefixCheck[sizeof(CAITargeting) == 0x108 ? 1 : -1];
// Original enum tags. Behavior selectors match each behavior class's GetType
// return; all member names are descriptive.
enum EBehaviorType {
    BehaviorTypeBuddyLead = 1, BehaviorTypeBuddyFollow = 2, BehaviorTypeMountedMachineGun = 3,
    BehaviorTypeCoverpoint = 4, BehaviorTypeScripted = 5, BehaviorTypePistol = 6, BehaviorTypeSword = 8,
    BehaviorTypeGrenadeResponse = 10
};
enum EAIHighLevelState { AIHighLevelStateUnknown = 0 };
enum ECapability { CapabilityUnknown = 0 };
enum ECollisionTestDirection { CollisionTestDirectionUnknown = 0 };
class CAIBehavior;
// Method-only view of the embedded tactics module.
class CAITactics {
public:
    int FindTacticalPoint(CVector3 *, float);
};
// Original enum tag; selector names are descriptive.
enum aistatus_var { aistatus_var_team = 4 };
// Method-only view of the embedded AI status block.
class CAIStatus {
public:
    int GetVarValue(aistatus_var) const;
    void SetVarValue(aistatus_var, int);
};
// Observed script-object, position and forward-vector prefix and the embedded
// AI modules through the spline-path module at +0x2f0. Module offsets come from
// the built-ins that pass their addresses; the gaps between them stay opaque.
// This is not a complete allocation, inheritance model or virtual interface.
class CAIObject {
public:
    unsigned char unknown00[0x0c];
    BSObject *scriptObject;
    unsigned char unknown10[0x20];
    CVector3 position;
    unsigned char unknown40[0x20];
    CVector3 forward;
    unsigned char unknown70[0x30];
    CAITargeting targeting; // +0xa0
    unsigned char unknown1a8[0x1b0 - 0xa0 - sizeof(CAITargeting)];
    CAILocomotion locomotion; // +0x1b0
    unsigned char unknown224[0x230 - 0x1b0 - sizeof(CAILocomotion)];
    CAIAreaPathFinding areaPathFinding; // +0x230
    unsigned char unknown254[0x2f0 - 0x230 - sizeof(CAIAreaPathFinding)];
    CAISplinePathModule splinePath; // +0x2f0
    unsigned char unknown310[0x374 - 0x2f0 - sizeof(CAISplinePathModule)];
    BSObject *previousBehaviorScript; // +0x374
    unsigned char unknown378[0x594 - 0x378];
    int cookie;          // +0x594; script-owned value (BIFunc_GetCookie/SetCookie).
    unsigned char team;  // +0x598; set by BIFunc_AISetTeam.
    unsigned char unknown599[0x5e0 - 0x599];
    unsigned int statusFlags2; // +0x5e0; bit 0 cleared with flag 2.
    unsigned char unknown5e4[4];
    unsigned int statusFlags;  // +0x5e8; set and cleared by BIFunc_AISendFlag.
    CAIStatus status;    // +0x5ec
    unsigned char unknown5ed[0x624 - 0x5ed];
    float accuracyNear;  // +0x624; set per accuracy level by BIFunc_SetAccuracy.
    unsigned char unknown628[4];
    float accuracyFar;   // +0x62c
    unsigned char unknown630[0x638 - 0x630];
    float hearingRadius; // +0x638; set by BIFunc_AISetHearingRadius.
    unsigned char unknown63c[0x6e4 - 0x63c];
    int inShadow;        // +0x6e4
    float shadowAmount;  // +0x6e8
    int matchList;       // +0x6ec; first word of the soldier match list returned by address.
    unsigned char unknown6f0[0x770 - 0x6f0];
    struct SnapState {
        unsigned char unknown00[0x10];
        CVector3 position; // +0x10
        float angle;       // +0x20
        int useAngle;      // +0x24
        int active;        // +0x28
    } snap;              // +0x770; set by BIFunc_AISnapToMovePoint*.
    unsigned char unknown79c[0x7a0 - 0x79c];
    CAITactics tactics;  // +0x7a0
    CAIBehavior *FindMatchingBehavior(EBehaviorType);
    float GetDistanceSquaredXYReal(CAITarget);
    // Original nested enum tag; its members are unknown.
    enum ECollisionTestStyle { CollisionTestStyleDefault = 0 };
    float PerformCollisionTest(ECollisionTestDirection, float, ECollisionTestStyle);
    void SetupFieldOfViewModeList(aistatus_match *, int, float, float);
    void SetupLineOfSightModeList(aistatus_match *, int, float);
    void SetupFieldOfViewModeObject(const CAITarget &, float, float);
    void SetupLineOfSightModeObject(const CAITarget &, float);
    void EnableBehavior(EBehaviorType);
    void DisableBehavior(EBehaviorType);
    int IsBehaviorEnabled(EBehaviorType);
    int ChooseBehavior();
    int ChooseNewBehavior();
    int StartBehavior();
    void StopBehavior(bool);
    void ClearBehavior();
    int GetBehaviorCapability(ECapability);
    int GetPreviousBehaviorType();
    int GetNextBehaviorType();
    int GetNextBehaviorScript();
    void SetHighLevelState(EAIHighLevelState);
    int IsNearbyPositionOnAStarMesh(ECollisionTestDirection, float);
    void SetupFieldOfViewCorpse(float, float);
    int FindRunAwayPoint(const ISceneNode *, float, CVector3 *, bool);
};
// Doodad and filter prefixes reached by the script built-ins: the doodad's first
// word is its filter, and the filter's second word is the AI object.
class CAIFilterObject {
public:
    unsigned char unknown00[4];
    CAIObject *object;
};
class CAIDoodad {
public:
    CAIFilterObject *filter;
};
// Member view of the soldier AI object returned by CAIObject slot 2.
class CAIThreatEvaluation {
public:
    void FinishedInvestigation();
    void AddCorpseThreat(BSObject *);
};
class CAISoldierObject {
public:
    unsigned char unknown_0[0x7b0];
    CAIThreatEvaluation threatEvaluation; // +0x7b0
};
class IMovingSceneNode;
// Virtual-interface view of CAIObject: its table pointer follows the data at
// +0x72c. Callers convert the CAIObject pointer they already hold.
class CAIObjectInterface {
public:
    unsigned char unknown_0[0x72c];
    virtual ~CAIObjectInterface();
    virtual CAISoldierObject *AsAISoldierObject();
    virtual void *AsAIPlayerObject();
    virtual void *AsAITankObject();
    virtual void PreUpdate();
    virtual void Update(float);
    virtual void UpdateFromGameObject(IMovingSceneNode *);
    virtual void OnStateChange(EAIHighLevelState, EAIHighLevelState);
    virtual void OnDeath();
    virtual void SetupTargetMatchList(aistatus_match *, int);
    virtual int ChooseTarget(float);
};
enum EGrenadeThrow { GrenadeThrowUnknown = 0 };
int SetUpGrenadeShot(EGrenadeThrow, ISceneNode *);
#endif
