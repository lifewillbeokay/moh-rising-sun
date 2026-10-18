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
// Observed prefix of a buddy-lead checkpoint record.
struct BuddyLeadCheckpoint {
    int unknown_0;
    int value; // +0x4; returned by BIFunc_GetBuddyLeadCheckpointFromID.
};
class CBuddyLeadBehavior { // Type 1.
  public:
    static BuddyLeadCheckpoint *GetBuddyLeadCheckpointFromID(int);
    void HandleCheckpointReachedByBuddy(int);
    static void HandleCheckpointReachedByPlayer(int);
};

class CBuddyFollowBehavior { // Type 2.
  public:
    void CantReachTarget();
    void FinishedMovingAway();
    void SetDistances(float, float, float, float);
    void EnterBuddyTeleportArea(int);
    void ExitBuddyTeleportArea(int);
    int IsBuddyInArea(int);
    float UpdateDistanceToPlayer();
    static void RegisterBuddyFollowObject(BSObject *, float, float, float, float);
};

class CPistolBehavior { // Type 6.
  public:
    // The table pointer follows twelve bytes of base data; slots 1-13 in the
    // order of the original table at 0x802e7e20.
    unsigned char unknown00[8];
    int field_08; // Checked with GetScriptObject by BIFunc_HasPistol.
    virtual ~CPistolBehavior();
    virtual float GetCurrentHeuristicScore();
    virtual float GetHeuristicUpdateFrequency();
    virtual void UpdateHeuristicScore();
    virtual void StartBehavior();
    virtual void StopBehavior(bool);
    virtual void ReleaseResources();
    virtual int GetCapabilities(ECapability);
    virtual int GetType();
    virtual unsigned int GetCRC();
    virtual void Enable();
    virtual void Disable();
    virtual BSObject *GetScriptObject();
};

class CMountedMachineGunBehavior { // Type 3.
  public:
    // The table pointer follows twelve bytes of base data; slots 1-4 in the
    // order of the original behavior tables (e.g. 0x802e8180).
    unsigned char unknown00[12];
    virtual ~CMountedMachineGunBehavior();
    virtual float GetCurrentHeuristicScore();
    virtual float GetHeuristicUpdateFrequency();
    virtual void UpdateHeuristicScore();
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
};

class CCoverpointBehavior { // Type 4.
  public:
    // The table pointer follows twelve bytes of base data; slots 1-4 in the
    // order of the original behavior tables (e.g. 0x802e8180).
    unsigned char unknown00[12];
    virtual ~CCoverpointBehavior();
    virtual float GetCurrentHeuristicScore();
    virtual float GetHeuristicUpdateFrequency();
    virtual void UpdateHeuristicScore();
    int StartCoverPointWalk();
    void StopCoverPointWalk();
    void Arrived();
    void EndCoverpoint();
    void ForceToCoverPoint();
    static void InvalidateCoverPointByID(int);
    static void RegisterCoverpoint(TriggerObject *);
};

class CScriptedBehavior { // Type 5.
  public:
    unsigned char unknown00[0x4c];
    int scriptedType; // +0x4c; read and written by the scripted-behavior type built-ins.
    void SetAllowedHighLevelState(EAIHighLevelState);
    void SetCapability(ECapability, bool);
    void StartScript(BSObject *);
    void StopScript();
};

class CSwordBehavior { // Type 8.
  public:
    int IsSwordDrawn();
};

class CGrenadeResponseBehavior { // Type 10.
  public:
    unsigned char unknown_0[0x24];
    int caught; // +0x24; set by BIFunc_CatchGrenade.
    void FinishedResponse(CThrownBullet *);
};

// Static scripted-gag entry points.
class CScriptedGag {
  public:
    static void RegisterGagAction(BSObject *, const char *, int);
    static void StartGag(BSObject *, const char *, int);
    static void StartGag(BSObject *, unsigned int, int);
};

// Method-only view of a cover point; the script object's native trigger is one.
class CCoverPoint {
  public:
    void Invalidate();
};

// Static lookup used by BIFunc_AIFindTank; script object at +0x0c as in CAIObject.
class CAITankObject {
  public:
    unsigned char unknown00[0x0c];
    BSObject *scriptObject;
    static CAITankObject *FindTank();
};

#endif
