// AI-assisted scoped reconstruction from GR8E69; see docs/Script.md.
#ifndef GAME_AI_BEHAVIOR_H
#define GAME_AI_BEHAVIOR_H
#include "AITargeting.h"
class CThrownBullet;
class TriggerObject;
class CStaticObject;

// Method-only views of the behaviors that script built-ins reach through
// CAIObject::FindMatchingBehavior. Each class's GetType returns the selector
// noted beside it. Storage, bases and virtual interfaces are not declared; the
// built-ins convert the returned behavior pointer without adjustment.
class CCoverPoint;
// A buddy-lead checkpoint record: only its prefix is observed. The tag is original.
struct Checkpoint_t {
    TriggerObject *trigger; // +0x0
    int value;              // +0x4; returned by BIFunc_GetBuddyLeadCheckpointFromID.
    int reached;            // +0x8
};
typedef Checkpoint_t BuddyLeadCheckpoint;
class CBuddyLeadBehavior : public CBehavior { // Type 1.
  public:
    CBuddyLeadBehavior();
    virtual ~CBuddyLeadBehavior();
    virtual float GetCurrentHeuristicScore();
    virtual int GetHeuristicUpdateFrequency();
    virtual int UpdateHeuristicScore();
    virtual int StartBehavior();
    virtual void StopBehavior(bool);
    virtual int GetCapabilities(ECapability);
    virtual int GetType();
    static BuddyLeadCheckpoint *GetBuddyLeadCheckpointFromID(int);
    void HandleCheckpointReachedByBuddy(int);
    static void HandleCheckpointReachedByPlayer(int);
    static void CacheBuddyLeadTriggers();
    int checkpointDone;   // +0x10
    int finished;         // +0x14
    float heuristicScore; // +0x18
    Checkpoint_t *currentCheckpoint; // +0x1c
};

class CBuddyFollowBehavior : public CBehavior { // Type 2.
  public:
    CBuddyFollowBehavior(CAIActivityZone *);
    virtual ~CBuddyFollowBehavior();
    virtual void Enable();
    virtual void Disable();
    virtual float GetCurrentHeuristicScore();
    virtual int GetHeuristicUpdateFrequency();
    virtual int UpdateHeuristicScore();
    virtual int StartBehavior();
    virtual void StopBehavior(bool);
    virtual int GetCapabilities(ECapability);
    virtual int GetType();
    void BumpPlayer();
    void CantReachTarget();
    void FinishedMovingAway();
    void SetDistances(float, float, float, float);
    void EnterBuddyTeleportArea(int);
    void ExitBuddyTeleportArea(int);
    int IsBuddyInArea(int);
    float UpdateDistanceToPlayer();
    float GetStopDistance();
    float GetStartDistance();
    static void RegisterBuddyFollowObject(BSObject *, float, float, float, float);
    float heuristicScore;           // +0x10
    float stopDistance;             // +0x14; state 4 uses stopAttackDistance instead.
    float moveThreshold;            // +0x18; state 4 uses breakOffAttackDistance instead.
    float breakOffAttackDistance;   // +0x1c
    float stopAttackDistance;       // +0x20
    int stopped;                    // +0x24; set by StopBehavior, cleared by StartBehavior.
    int cantReachTarget;            // +0x28
    float cantReachExpireTime;      // +0x2c
    int distancesSet;               // +0x30; keeps Enable from restoring the defaults.
    CAIActivityZone *activityZone;  // +0x34
    int teleportArea;               // +0x38
    int teleportAreaCount;          // +0x3c
    int movingAway;                 // +0x40; set by BumpPlayer.
    float movingAwayExpireTime;     // +0x44
    static BSObject *m_pBuddyFollowObject;
    static float m_defaultStopDistance;
    static float m_defaultMoveThreshold;
    static float m_defaultBreakOffAttackDistance;
    static float m_defaultStopAttackDistance;
};

class CPistolBehavior : public CBehavior { // Type 6.
  public:
    CPistolBehavior();
    virtual ~CPistolBehavior();
    virtual float GetCurrentHeuristicScore();
    virtual int GetHeuristicUpdateFrequency();
    virtual int UpdateHeuristicScore();
    virtual int StartBehavior();
    virtual void StopBehavior(bool);
    virtual int GetCapabilities(ECapability);
    virtual int GetType();
    virtual unsigned int GetCRC();
    virtual BSObject *GetScriptObject();
    virtual void SetWeaponScript(BSObject *);
    EAIHighLevelState highLevelState; // +0x10; passed with the start and stop events.
    float heuristicScore; // +0x14
    BSObject *weaponScript; // +0x18
};

class CRifleBehavior : public CBehavior { // Type 7.
  public:
    CRifleBehavior();
    virtual ~CRifleBehavior();
    virtual float GetCurrentHeuristicScore();
    virtual int GetHeuristicUpdateFrequency();
    virtual int UpdateHeuristicScore();
    virtual int StartBehavior();
    virtual void StopBehavior(bool);
    virtual int GetCapabilities(ECapability);
    virtual int GetType();
    virtual unsigned int GetCRC();
    virtual BSObject *GetScriptObject();
    virtual void SetWeaponScript(BSObject *);
    EAIHighLevelState highLevelState; // +0x10; passed with the start and stop events.
    float heuristicScore; // +0x14
    BSObject *weaponScript; // +0x18
};

class CNullWeaponBehavior : public CBehavior { // Type 9.
  public:
    CNullWeaponBehavior();
    virtual ~CNullWeaponBehavior();
    virtual float GetCurrentHeuristicScore();
    virtual int GetHeuristicUpdateFrequency();
    virtual int UpdateHeuristicScore();
    virtual int StartBehavior();
    virtual void StopBehavior(bool);
    virtual int GetCapabilities(ECapability);
    virtual int GetType();
    virtual unsigned int GetCRC();
    virtual BSObject *GetScriptObject();
    virtual void SetWeaponScript(BSObject *);
    EAIHighLevelState highLevelState; // +0x10; passed with the start and stop events.
    float heuristicScore; // +0x14
    BSObject *weaponScript; // +0x18
};

class CPolemineBehavior : public CBehavior { // Type 7, as the rifle.
  public:
    CPolemineBehavior();
    virtual ~CPolemineBehavior();
    virtual float GetCurrentHeuristicScore();
    virtual int GetHeuristicUpdateFrequency();
    virtual int UpdateHeuristicScore();
    virtual int StartBehavior();
    virtual void StopBehavior(bool);
    virtual int GetCapabilities(ECapability);
    virtual int GetType();
    virtual unsigned int GetCRC();
    virtual BSObject *GetScriptObject();
    virtual void SetWeaponScript(BSObject *);
    float heuristicScore; // +0x10
    BSObject *weaponScript; // +0x14
};

class TriggerObject;
class CMountedMachineGunBehavior : public CBehavior { // Type 3.
  public:
    CMountedMachineGunBehavior();
    virtual ~CMountedMachineGunBehavior();
    virtual float GetCurrentHeuristicScore();
    virtual int GetHeuristicUpdateFrequency();
    virtual int UpdateHeuristicScore();
    virtual int StartBehavior();
    virtual void StopBehavior(bool);
    virtual void ReleaseResources();
    virtual int GetCapabilities(ECapability);
    virtual int GetType();
    virtual BSObject *GetScriptObject();
    void SetBehaviorStyle(EAIHighLevelState);
    int DoIOwnMMG(TriggerObject *);
    void Arrived();
    void CantReachMMG();
    void HitFromBehind();
    void ForceToMMG();
    void ForceToMMGFeeder();
    static void DeregisterGun(int);
    static int GetFeederByLinkCode(int);
    static CStaticObject *GetGunByLinkCode(int);
    static void RegisterFeeder(int, BSObject *);
    static void RegisterGun(int, CStaticObject *);
    static void DeregisterAllGuns();
    static void DeregisterAllFeeders();
    static void DeregisterMMGTriggers();
    static void RegisterMMGTriggers();
    EAIHighLevelState behaviorStyle; // +0x10; the name's event argument.
    float heuristicScore;            // +0x14
    BSObject *gunScript;             // +0x18
    unsigned char unknown1c[0x24 - 0x1c];
    TriggerObject *bidTrigger;       // +0x24; withdrawn by the destructor.
    int arrived;                     // +0x28
    int stopping;                    // +0x2c
    int forced;                      // +0x30
    int cantReach;                   // +0x34
    float retryTime;                 // +0x38; set by CantReachMMG and HitFromBehind.
    float field3c;                   // +0x3c
    int hitFromBehind;               // +0x40
    int field44;                     // +0x44
};

class CCoverpointBehavior : public CBehavior { // Type 4.
  public:
    CCoverpointBehavior();
    virtual ~CCoverpointBehavior();
    static void UpdateCoverpointsOutOfArea(const ISceneNode *);
    virtual float GetCurrentHeuristicScore();
    virtual int GetHeuristicUpdateFrequency();
    virtual int UpdateHeuristicScore();
    virtual int StartBehavior();
    virtual void StopBehavior(bool);
    virtual void ReleaseResources();
    virtual int GetCapabilities(ECapability);
    virtual int GetType();
    virtual BSObject *GetScriptObject();
    CCoverPoint *ChooseCoverpoint();
    int CheckCoverpointUsefulness();
    void LeaveCoverPoint();
    void ForceMoveTargetUpdate();
    int StartCoverPointWalk();
    void WalkToCoverPoint();
    float GetAIValueByName(const char *);
    void StopCoverPointWalk();
    void Arrived();
    void EndCoverpoint();
    void ForceToCoverPoint();
    static void InvalidateCoverPointByID(int);
    static void RegisterCoverpoint(TriggerObject *);
    static void ResetCoverpoints();
    float heuristicScore;      // +0x10
    unsigned char unknown14[4];
    BSObject *coverScript;     // +0x18; the chosen cover point's script object.
    int arrived;               // +0x1c
    int stopping;              // +0x20; set by StopBehavior, cleared by ReleaseResources.
    int ended;                 // +0x24
    CCoverPoint *coverPoint;   // +0x28
    CAISoldierObject *soldier; // +0x2c
    float field30;             // +0x30; cleared by the constructor.
    unsigned char unknown34[0x40 - 0x34];
    CVector3 moveTarget;       // +0x40
    int forced;                // +0x50
};

class CScriptedBehavior : public CBehavior { // Type 5.
  public:
    CScriptedBehavior();
    virtual ~CScriptedBehavior();
    virtual float GetCurrentHeuristicScore();
    virtual int GetHeuristicUpdateFrequency();
    virtual int UpdateHeuristicScore();
    virtual int StartBehavior();
    virtual void StopBehavior(bool);
    virtual void ReleaseResources();
    virtual int GetCapabilities(ECapability);
    virtual int GetType();
    virtual BSObject *GetScriptObject();
    void SetAllowedHighLevelState(EAIHighLevelState);
    void SetCapability(ECapability, bool);
    void StartScript(BSObject *);
    void StopScript();
    BSObject *script;        // +0x10
    BSObject *pendingScript; // +0x14; started when StopBehavior ends the current script.
    int scriptActive;        // +0x18; makes the behavior score at all.
    int scoreChanged;        // +0x1c; read and cleared by UpdateHeuristicScore.
    int capabilities[11];    // +0x20; indexed by ECapability.
    int scriptedType;        // +0x4c; read and written by the scripted-behavior type built-ins.
    int allowedStateMask;    // +0x50; bit per high-level state; 0xffff allows every state.
};

class CSwordBehavior : public CBehavior { // Type 8.
  public:
    CSwordBehavior();
    virtual ~CSwordBehavior();
    virtual float GetCurrentHeuristicScore();
    virtual int GetHeuristicUpdateFrequency();
    virtual int UpdateHeuristicScore();
    virtual int StartBehavior();
    virtual void StopBehavior(bool);
    virtual int GetCapabilities(ECapability);
    virtual int GetType();
    virtual unsigned int GetCRC();
    virtual BSObject *GetScriptObject();
    virtual void SetWeaponScript(BSObject *);
    int IsSwordDrawn();
    void SetSwordDrawn(bool);
    int swordDrawn; // +0x10
    float heuristicScore; // +0x14
    BSObject *weaponScript; // +0x18
};

class CGrenadeResponseBehavior : public CBehavior { // Type 10.
  public:
    virtual ~CGrenadeResponseBehavior();
    virtual float GetCurrentHeuristicScore();
    virtual int GetHeuristicUpdateFrequency();
    virtual int UpdateHeuristicScore();
    virtual int StartBehavior();
    virtual void StopBehavior(bool);
    virtual void ReleaseResources();
    virtual int GetCapabilities(ECapability);
    virtual int GetType();
    virtual BSObject *GetScriptObject();
    void FinishedResponse(CThrownBullet *);
    static void Initialize();
    static void Shutdown();
    unsigned char unknown10[0x18 - 0x10];
    BSObject *responseScript; // +0x18; destroyed by ReleaseResources.
    unsigned char unknown1c[0x24 - 0x1c];
    int caught; // +0x24; set by BIFunc_CatchGrenade.
    unsigned char unknown28[0x2c - 0x28];
    float heuristicScore; // +0x2c
};

// A cover point; the script object's native trigger is one. Its first word holds the state
// bits below; their names follow the accessors.
class CCoverPoint {
  public:
    void InitFlags();
    int GetStyle() const;
    void GetModifiedPosition(CVector3 &) const;
    CVector3 GetPeekLocation();
    int IsOccupied() const;
    int IsValid() const;
    int IsAvailable() const;
    int FlagAsOccupied();
    void FlagAsUnoccupied();
    void SetOutOfArea(bool);
    void Invalidate();

    unsigned int unknownHigh : 16;
    unsigned int unknownBits : 13;
    unsigned int outOfArea : 1;
    unsigned int invalid : 1;
    unsigned int occupied : 1;
};

// Static lookup used by BIFunc_AIFindTank; script object at +0x0c as in CAIObject.
class CAITankObject {
  public:
    unsigned char unknown00[0x0c];
    BSObject *scriptObject;
    unsigned char unknown10[0x730 - 0x10];
    float health;      // +0x730
    float maxHealth;   // +0x734
    aistatus_match targetMatchList[1]; // +0x738; extent unknown.
    unsigned char unknown744[0x768 - 0x744];
    int field_768;     // +0x768; cleared by Init.
    unsigned char unknown76c[0x7a0 - 0x76c];
    ISceneNode *sceneNode; // +0x7a0; the tank scene object.
    unsigned char unknown7a4[1968 - 0x7a4]; // The allocation is 1968 bytes.
    static CAITankObject *FindTank();
    CAITankObject(void *);
    void *operator new(unsigned int, void *);
    void operator delete(void *);
    void Init();
    void SetInitialStatus();
    void UpdateStatus();
    void UpdateFromGameObject(IMovingSceneNode *);
    void PreUpdate();
    void SetupTargetMatchList(aistatus_match *, int);
    int ChooseTarget(float);
    ISceneNode *GetSceneNode();
    CAITankObject *AsAITankObject();
};

#endif
