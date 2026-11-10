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
#include "LocationTargetInfo.h"
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
// The base of the AI object's modules; only its table pointer and virtual destructor are known.
class CAIModule {
public:
    virtual ~CAIModule();
};
class CAITargeting : public CAIModule {
public:
    CAITargeting(CAIPhysics *, CAIObjectParameters *, ISceneNode *);
    virtual ~CAITargeting();
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
    CVector3 unknownVector50;
    unsigned char unknown60[0x10];
    CVector3 modifiedPosition;
    CVector3 unknownVector80;
    CVector3 unknownVector90;
    unsigned char unknownA0[0x10];
    CVector3 positionOffsetMin; // +0xb0; a random offset scaled from the parameters. The misspelled method names it.
    CVector3 unknownVectorC0;
    CVector3 unknownVectorD0;
    unsigned char unknownE0[0x10];
    int unknownF0;
    int alerted; // +0xf4; set to 1 by BIFunc_AISendFlag flag 1.
    float targetUpdateTime; // +0xf8; written after the script-target update.
    int blindFireMode; // +0xfc; observed selectors 0, 1 and 2.
    int weapon; // +0x100; passed to AdjustAimPosition.
    AIFILTER_WEAPON_AIM_MODE aimMode; // +0x104.
    float inverseBulletSpeed; // +0x108; written by the soldier filter.
    CAITarget GetTarget() const { return target; }
    void GetLastSeenTargetPosition(CVector3 &);
    void GetModifiedTargetPosition(CVector3 &);
    void SetLastSeenTargetPosition(const CVector3 &);
    int IsLastSeenTargetPositionSet();
    int CouldVisionSeeTarget();
    void Update();
    void UpdatePostionOffsetMin();
    void CalculateTargetPosition();
    bool UpdateReactionTime();
    void UpdateTargetPosition();
    void UpdateNonAITargetPosition();
    void SetNonAITarget(BSObject *);
    CAIObject *SetTarget(CAIObject *); // Returns the current target (its body loads it last).
    void AdjustAimPosition(CVector3 *, int, AIFILTER_WEAPON_AIM_MODE *);
    void AdjustForSniperAimMode(CVector3 *, AIFILTER_WEAPON_AIM_MODE);
    void AdjustForBazookaAimMode(CVector3 *, AIFILTER_WEAPON_AIM_MODE);
    void ChooseSniperAimMode(AIFILTER_WEAPON_AIM_MODE *);
    void ChooseBazookaAimMode(AIFILTER_WEAPON_AIM_MODE *);
    void GetAdjustedAimTargetPosition(CVector3 &);
    bool DoBlindFire();
    float GetDistanceToTargetSquaredXYZReal();
    BSObject *GetTargetScript() const;
    static void RegisterLocationTarget(int, bool, ETeam, BSObject *);
    static void DeRegisterLocationTarget(int);
    static void ResetLocationTargets();
    static _LocationTargetInfo GetLocationTarget(int);
    bool IsNonAITarget() const;
    bool HasValidTarget() const;
    bool GetTargetGuessPosition(CVector3 &);
};
typedef char AITargetingPrefixCheck[sizeof(CAITargeting) == 0x10c ? 1 : -1];
// Original enum tags. Behavior selectors match each behavior class's GetType
// return; all member names are descriptive.
enum EBehaviorType {
    BehaviorTypeBuddyLead = 1, BehaviorTypeBuddyFollow = 2, BehaviorTypeMountedMachineGun = 3,
    BehaviorTypeCoverpoint = 4, BehaviorTypeScripted = 5, BehaviorTypePistol = 6, BehaviorTypeSword = 8,
    BehaviorTypeGrenadeResponse = 10
};
enum EAIHighLevelState { AIHighLevelStateUnknown = 0 };
// Selector names are descriptive: only their values are known.
enum ECapability { Capability0 = 0, Capability1, Capability2, Capability3, Capability4, Capability5, Capability6, Capability7 };
enum ECollisionTestDirection { CollisionTestDirectionUnknown = 0 };
class CAIBehavior;
// The behavior base class: its table pointer follows its data, at +0xc. Method names
// come from the symbol table; member names and the pure slots' return types are inferred.
class CBehavior {
public:
    CAIObject *aiObject; // +0x00; cleared here and set by the owner.
    int framesSinceUpdate; // +0x04; accumulated by Update.
    int enabled; // +0x08
    CBehavior();
    virtual ~CBehavior();
    virtual float GetCurrentHeuristicScore() = 0;
    virtual int GetHeuristicUpdateFrequency() = 0;
    virtual int UpdateHeuristicScore() = 0;
    virtual int StartBehavior() = 0;
    virtual void StopBehavior(bool) = 0;
    virtual void ReleaseResources();
    virtual int GetCapabilities(ECapability) = 0;
    virtual int GetType();
    virtual unsigned int GetCRC();
    virtual void Enable();
    virtual void Disable();
    virtual BSObject *GetScriptObject();
    int Update(int, bool);
};
// Method-only view of the embedded tactics module.
class CAIObject;
class CAITargeting;
class CAITactics : public CAIModule {
public:
    CAITactics(CAIPhysics *, CAITargeting *, CAIObject *);
    virtual ~CAITactics();
    int FindTacticalPoint(CVector3 *, float);
    float GetTacticalPositionScore(const CVector3 &, float);
    float AggressionScore(const CVector3 &, float);
    float AlliesScore(const CVector3 &, float);
    float AngleFromTargetScore(const CVector3 &, float);
    CAIPhysics *physics;     // +0x04
    CAITargeting *targeting; // +0x08
    CAIObject *owner;        // +0x0c
};
// Original enum tag; selector names are descriptive.
enum aistatus_var { aistatus_var_team = 4 };
// Original enum tag; its members are unknown.
enum aistatus_comparison { aistatus_comparison_equal = 0 };
struct aistatus_match {
    aistatus_var var;
    int value;
    aistatus_comparison comparison;
};
// The embedded AI status block. Variables 0-4, 6 and 11 read the array directly;
// the rest are computed between s_paioAttacker and s_pTarget.
class CAIObject;
class CAITarget;
class CAIStatus {
public:
    CAIStatus();
    ~CAIStatus();
    int GetVarValue(aistatus_var) const;
    void SetVarValue(aistatus_var, int);
    int MatchStatus(aistatus_var, int, aistatus_comparison);
    int MatchStatus(aistatus_match *);
    int MatchStatusList(aistatus_match *, int);
    int values[12];
    static CAIObject *s_paioAttacker;
    static CAITarget *s_pTarget;
};
// Observed script-object, position and forward-vector prefix and the embedded
// AI modules through the spline-path module at +0x2f0. Module offsets come from
// the built-ins that pass their addresses; the gaps between them stay opaque.
// This is not a complete allocation, inheritance model or virtual interface.
class CAIFilterObject;
class CPlayerObject;
class CAITankObject;
class IMovingSceneNode;
class CBehavior;
class CSoldierObject;
class CAISnap {
public:
    unsigned char unknown00[4];
    CSoldierObject *soldierObject; // +0x04
    unsigned char unknown08[8];
    CVector3 position; // +0x10
    float angle;       // +0x20
    int useAngle;      // +0x24
    int active;        // +0x28
    void Init();
    void Update(float);
};
// One high-level state's behavior list (four are stored from +0x380).
struct AIBehaviorList {
    CBehavior *behaviors[32];
    int count;
};
class CAIObject {
public:
    CAIPhysics *Physics() { return (CAIPhysics *)physicsPrefix; }
    CAIObject *nextGlobal;   // +0x00; the global AI object list.
    CAIFilterObject *filter; // +0x04; the owning filter.
    IMovingSceneNode *sceneNode; // +0x08; set by UpdateFromGameObject.
    BSObject *scriptObject;
    EAIHighLevelState highLevelState; // +0x10
    unsigned char unknown14[0xc];
    unsigned char physicsPrefix[0x10]; // +0x20; start of the CAIPhysics module.
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
    unsigned char unknown310[0x370 - 0x2f0 - sizeof(CAISplinePathModule)];
    CBehavior *previousBehavior;      // +0x370
    BSObject *previousBehaviorScript; // +0x374
    CBehavior *currentBehavior;       // +0x378
    CBehavior *nextBehavior;          // +0x37c
    AIBehaviorList behaviorLists[4]; // +0x380; indexed by high-level state minus one.
    float lastUpdateTime; // +0x590; written from the AI clock by Update.
    int cookie;          // +0x594; script-owned value (BIFunc_GetCookie/SetCookie).
    unsigned char team;  // +0x598; set by BIFunc_AISetTeam.
    unsigned char unknown599; // +0x599; 4 for soldiers, 7 for tanks and 5 for players.
    short id;            // +0x59a; the trigger's "ID".
    unsigned char unknown59c[0x5e0 - 0x59c];
    unsigned int statusFlags2; // +0x5e0; bit 0 cleared with flag 2.
    unsigned int previousStatusFlags2; // +0x5e4
    unsigned int statusFlags;  // +0x5e8; set and cleared by BIFunc_AISendFlag.
    CAIStatus status;    // +0x5ec
    int targeterCount;   // +0x61c; objects targeting this one, per CAITargeting::SetTarget.
    float reactionDelay; // +0x620; the trigger's "ReactionDelay" times 0.1.
    float accuracyNear;  // +0x624; set per accuracy level by BIFunc_SetAccuracy.
    float trackingDelay; // +0x628; from the trigger's "TrackingDelay" selector.
    float accuracyFar;   // +0x62c
    unsigned char unknown630[0x638 - 0x630];
    float hearingRadius; // +0x638; set by BIFunc_AISetHearingRadius.
    unsigned char unknown63c[4];
    CAITarget visionTarget;      // +0x640; last match in field-of-view list mode.
    CAITarget fieldOfViewTarget; // +0x64c; field-of-view object mode.
    unsigned char unknown658[0x698 - 0x658];
    float fieldOfViewListRange;    // +0x698
    float fieldOfViewObjectRange;  // +0x69c
    float fieldOfViewCorpseRange;  // +0x6a0
    float fieldOfViewObjectAngle;  // +0x6a4; scaled from the caller's value.
    float fieldOfViewListAngle;    // +0x6a8
    float fieldOfViewCorpseAngle;  // +0x6ac
    aistatus_match visionMatchList[4]; // +0x6b0
    int visionMatchCount;          // +0x6e0
    int inShadow;        // +0x6e4
    float shadowAmount;  // +0x6e8
    int matchList;       // +0x6ec; first word of the soldier match list returned by address.
    unsigned char unknown6f0[0x730 - 0x6f0];
    float hitPoints;     // +0x730; the trigger's "HitPoints".
    float maxHitPoints;  // +0x734; set with hitPoints, meaning inferred.
    CPlayerObject *owner; // +0x738; set by CPlayerObject::InitAI.
    unsigned char unknown73c[0x770 - 0x73c];
    typedef CAISnap SnapState;
    CAISnap snap;        // +0x770; set by BIFunc_AISnapToMovePoint*.
    unsigned char unknown79c[0x7a0 - 0x79c];
    CAITactics tactics;  // +0x7a0
    static CAIObject *GetGlobalListStart();
    void PreUpdate();
    void Update(float);
    void UpdateFromGameObject(IMovingSceneNode *);
    void Init();
    void SetAIStatusVar(aistatus_var, int);
    int IsObjectInFieldOfView(CAIObject &, float, float);
    int IsObjectInFieldOfView(const CVector3 &, float, float, bool, ISceneNode *);
    int IsObjectInFieldOfView(const CVector3 &, const CVector3 &, const CVector3 &, float, float, bool, ISceneNode *);
    int IsObjectOccluded(const CVector3 &, const CVector3 &, bool, ISceneNode *);
    CAITarget *CheckFieldOfViewModeObject();
    int CheckFieldOfViewModeList(CAITarget &);
    int SearchNonAITargetsInFieldOfView(CAITarget &);
    int CheckFieldOfViewCorpse(CAITarget &);
    CAITarget *CheckLineOfSightModeObject();
    int CheckLineOfSightModeList(CAITarget &);
    int IsTargetInLineOfSight(float);
    int IsObjectInLineOfSight(const CVector3 &, float, bool, const CAITarget *);
    int IsObjectInLineOfSight(const CVector3 &, const CVector3 &, float, bool) const;
    int IsObjectReachable(const CVector3 &);
    int IsObjectInLineOfSight(const CAITarget &, float);
    float GetHearingDistance();
    float GetAIValueByName(const char *);
    CBehavior *FindMatchingBehavior(EBehaviorType);
    CBehavior *FindMatchingBehaviorByCRC(int);
    float GetDistanceSquaredXYReal(CAITarget);
    // Original nested enum tag; its members are unknown.
    enum ECollisionTestStyle { CollisionTestStyleDefault = 0 };
    int ComputeStatusDistance(CAITarget *);
    int ComputeStatusExponentialDistance(CAITarget *);
    int ComputeStatusThreatValue(CAITarget *);
    int ComputeStatusLOSValue(CAITarget *);
    int ComputeStatusIsEnemy(CAITarget *);
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
    CBehavior *FindBestBehavior();
    CBehavior *FindMatchingBehviorFromCurrentState(EBehaviorType);
    void GetLocation(float *, float *, float *);
    void GetLocation(CVector3 *);
    float GetDistanceXYZReal(CAIObject *);
    float GetDistanceXYReal(CAIObject *);
    void *AsAISoldierObject();
    void *AsAIPlayerObject();
    void *AsAITankObject();
    void OnStateChange(EAIHighLevelState, EAIHighLevelState);
    void OnDeath();
    void SetupTargetMatchList(aistatus_match *, int);
    bool IsCrouching() const;
    int IsPlayer() const;
    int CheckIncomingFlagAll(unsigned int) const;
    int GetAIStatusVar(aistatus_var) const;
    void AddToGlobalList();
    void RemoveFromGlobalList();
    void RemovePointersTo(CAIObject *);
    int GetPreviousBehaviorType();
    int GetNextBehaviorType();
    BSObject *GetNextBehaviorScript();
    void SetHighLevelState(EAIHighLevelState);
    int IsNearbyPositionOnAStarMesh(ECollisionTestDirection, float);
    void SetupFieldOfViewCorpse(float, float);
    int FindRunAwayPoint(const ISceneNode *, float, CVector3 *, bool);
    static void ResetGlobalList();
    void UpdateHeuristics(int, bool);
    int IsValidTargetTeam(unsigned char);
    CAIObject *GetNextVisionTarget();
    int IsObjectInVisionDistance(const CVector3 &, float) const;
    int ChooseTarget(float);
};
// Doodad and filter prefixes reached by the script built-ins: the doodad's first
// word is its filter, and the filter's second word is the AI object.
// The filter's table pointer follows its leading members; +0x08 and +0x0c are not
// yet identified.
class CAIFilterObject {
public:
    virtual ~CAIFilterObject();
    void LinkGameObject(void *);
    CAIFilterObject(BSObject *);
    BSObject *GetScriptObject() const;
    void *GetBSGO() const;
    virtual void SetMoveTarget(const CVector3 &);
    void *gameObject;   // +0x00; the owning doodad, stored by LinkGameObject.
    CAIObject *object;  // +0x04
    BSObject *scriptObject; // +0x08
    void *bsgo;         // +0x0c; the script object's word at +0x20.
};
class CAIDoodad {
public:
    CAIDoodad();
    virtual ~CAIDoodad();
    virtual ISceneNode *GetSceneNode() const;
    virtual CAIObject *GetAIObject() const = 0;
    void Init();
    CAIFilterObject *filter;
};
// A stimulus reported to a soldier. Method, class and enum names are original; member
// names and the enumerators' meanings are inferred (see docs/Behaviors.md). The 16-byte
// alignment is inferred too: it gives the observed gaps before and after the embedded
// threat in ThreatInfo (+0x4 to +0x10, +0x2c to +0x30) and the slot address forms.
// The zone a soldier's behaviors stay inside; only its methods are declared here.
class CAIActivityZone {
public:
    void Disable();
    void UseMovingTarget(ISceneNode *, float);
    int IsPointUsable(const CVector3 &);
    unsigned char unknown00[4];
    int mode;           // +0x04; 0 anywhere, 1 around the node, 2 around the point.
    unsigned char unknown08[8];
    CVector3 point;     // +0x10
    ISceneNode *node;   // +0x20
    float radius;       // +0x24
};
class CAIThreat {
public:
    enum ThreatType { ThreatNone = 0 };
    CAIThreat();
    CAIThreat(ThreatType, const CVector3 &, BSObject *, bool);
    CVector3 position;
    ThreatType type;   // +0x10
    BSObject *source;  // +0x14
    bool directed;     // +0x18; a directed threat's score is not capped when scored.
} __attribute__((aligned(16)));
// Member view of the soldier AI object returned by CAIObject slot 2: the table pointer at
// +0 and the fifteen threat slots. The class's complete extent is not established.
class CAIThreatEvaluation {
public:
    struct ThreatInfo {
        int used;
        CAIThreat threat;   // +0x10
        int rank;           // +0x30; position after RankThreats.
        int reacted;        // +0x34; set once the owner has been told.
        float initialTime;  // +0x38; cleared with the slot and never read in these functions.
        float score;        // +0x3c
    };
    void AddThreat(const CAIThreat &);
    void Reevaluate();
    void AIDied(BSObject *);
    int HaveThreat();
    CVector3 &GetPositionOfMostImportantThreat();
    void PropagateSoundToMe(int, float, const CVector3 &, CAIObject *, bool);
    void Update(float);
    void FinishedInvestigation();
    void AddCorpseThreat(BSObject *);
    void OnStateChange(EAIHighLevelState, EAIHighLevelState);
    int InsertThreat(const CAIThreat &);
    void InitializeSlot(int, const CAIThreat &);
    void RankThreats();
    void ScoreInitialThreat(int);
    void ReactToThreat(ThreatInfo *);
    void ClearSlot(int);
    void DebugDraw();
    unsigned char unknown00[4];
    CAIObject *aiObject;     // +0x04
    CAIPhysics *physics;     // +0x08
    BSObject *scriptObject;  // +0x0c
    ThreatInfo slots[15];    // +0x10
    int count;               // +0x3d0
    ThreatInfo *mostImportant; // +0x3d4
};
class CSoldierObject;
class CAISoldierObject {
public:
    CAISoldierObject(void *);
    void *operator new(unsigned int, void *);
    void operator delete(void *);
    void Init();
    void SetSoldierObject(CSoldierObject *);
    void PreUpdate();
    void UpdateFromGameObject(IMovingSceneNode *);
    void Update(float);
    float GetAIValueByName(const char *);
    void SetInitialStatus();
    void UpdateStatus();
    void SetupTargetMatchList(aistatus_match *, int);
    ISceneNode *GetSceneNode();
    void OverrideLOSPosition(const CVector3 &);
    void StopOverrideLOSPosition();
    void OnStateChange(EAIHighLevelState, EAIHighLevelState);
    void OnDeath();
    CAISoldierObject *AsAISoldierObject();
    unsigned char unknown_0[0x190];
    int aimDirty; // +0x190; set when an aim modifier changes. Descriptive.
    unsigned char unknown_194[0x340 - 0x194];
    CAIActivityZone activityZone; // +0x340
    unsigned char unknown_after_zone[0x730 - 0x340 - sizeof(CAIActivityZone)];
    float health;    // +0x730
    float maxHealth; // +0x734
    aistatus_match targetMatchList[4]; // +0x738
    int targetMatchCount; // +0x768
    CSoldierObject *soldierObject; // +0x76c
    CAISnap snap; // +0x770
    unsigned char unknown_79c[0x7b0 - 0x79c];
    CAIThreatEvaluation threatEvaluation; // +0x7b0
    unsigned char unknown_after_threats[0xd10 - 0x7b0 - sizeof(CAIThreatEvaluation)];
    unsigned char coverpointStorage[0xdb8 - 0xd10]; // +0xd10; a CCoverpointBehavior. The allocation is 3536 bytes.
    int losOverride;               // +0xdb8
    unsigned char unknown_dbc[4];
    CVector3 losOverridePosition;  // +0xdc0
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
    virtual CAITankObject *AsAITankObject();
    virtual void PreUpdate();
    virtual void Update(float);
    virtual void UpdateFromGameObject(IMovingSceneNode *);
    virtual void OnStateChange(EAIHighLevelState, EAIHighLevelState);
    virtual void OnDeath();
    virtual void SetupTargetMatchList(aistatus_match *, int);
    virtual int ChooseTarget(float);
    virtual bool IsCrouching() const;
    virtual int IsPlayer() const;
    virtual void GetLookDirection(CVector3 &, CVector3 &);
    virtual int GetAIValueByName(const char *);
    virtual int IsObjectInLineOfSight(const CVector3 &, float, bool, const CAITarget *);
    virtual ISceneNode *GetSceneNode();
};
enum EGrenadeThrow { GrenadeThrowUnknown = 0 };
int SetUpGrenadeShot(EGrenadeThrow, ISceneNode *);
#endif
