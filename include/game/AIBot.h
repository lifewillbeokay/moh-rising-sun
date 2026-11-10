// AI-assisted reconstruction from GR8E69. Member views of the multiplayer bot classes:
// method and class names come from the symbol table, member names and offsets from
// their use in the reconstructed functions. This is not a complete class layout; see
// docs/Behaviors.md.
#ifndef GAME_AI_BOT_H
#define GAME_AI_BOT_H
#include "AITargeting.h"
#include "CMatrix.h"
class CWeapon;
class CAreaPathNode;
// Original enum tags; the member names are not known.
enum eBotState { BotStateUnknown = 0 };
enum eBotMoveState { BotMoveUnknown = 0 };
// A value-by-input graph; only the lookup is known.
class CGraph {
  public:
    float GetValue(float);
};
// Embedded in the bot at +0x1ac0, with its size up to the grenade pointer; the skills are read
// by the bot's decisions, and their meanings are not established.
class CBotCharacter {
  public:
    CBotCharacter();
    void Init();
    float attributes[7];   // +0x00; all set to one value by Init.
    unsigned char unknown1c[4];
    float skills[7];       // +0x20
    unsigned char unknown3c[0x48 - 0x3c];
    float field48;         // +0x48
    int field4c;           // +0x4c
    int field50;           // +0x50
    float field54;         // +0x54
    float field58;         // +0x58
};
// One entry of the bot's weapon table (40 bytes, from +0x1a20); the field names are inferred.
class CBotWeapon {
  public:
    float GetIdealRange(CBotCharacter *);
    float rating;      // +0x00; compared by ChooseWeapon and RetreatForAmmo.
    float rangeRating; // +0x04
    unsigned char unknown08[0x18 - 0x08];
    CGraph rangeGraph; // +0x18; looked up at 0 and 1 for the weapon's minimum and maximum range.
    unsigned char unknown19[0x24 - 0x19];
    CWeapon *weapon;   // +0x24
};
// One tracked enemy (eighty bytes, from the array stride).
struct EnemyInfo {
    int inView;         // +0x00; tested by EnemyInView.
    unsigned char unknown04[0x10 - 4];
    CVector3 position;  // +0x10
    unsigned char unknown20[0x30 - 0x20];
    float lastSeenTime; // +0x30
    unsigned char unknown34[0x3c - 0x34];
    float distance;     // +0x3c
    unsigned char unknown40[80 - 0x40];
};
class CBotObject;
class TriggerObject;
class MMG_Info {
  public:
    int IsInUse();
    void SetInUse(bool);
    void *gun;              // +0x00
    TriggerObject *trigger; // +0x04
    unsigned char unknown08[80 - 8];
};
// A camping point's look target (32 bytes, from the array stride). The fourth word of the position
// scales the look time in SetCampingLookat.
struct CampingLookat {
    CVector3 position;
    unsigned char unknown10[16];
};
// One camping point (48 bytes, from the array stride).
class CampingPoint {
  public:
    CVector3 *GetLookat(unsigned char);
    unsigned char unknown00[0x11];
    unsigned char type;                     // +0x11; 0 and 2 decide the crouch in BS_Enter_Camp.
    unsigned char lookatCount;              // +0x12
    unsigned char unknown13[0x16 - 0x13];
    unsigned char lookatIndices[48 - 0x16]; // +0x16
};
class CBotMan {
  public:
    void SetupCampingPoints();
    CBotObject *enemies[1];            // +0x000; indexed by GetEnemyAI, extent not established.
    unsigned char unknown004[0x60 - 4];
    MMG_Info mmgs[1];                  // +0x60; extent not established.
    unsigned char unknownb0[0x2e0 - 0xb0];
    CampingPoint campingPoints[1];  // +0x2e0; extent not established.
    unsigned char unknown310[0x8e0 - 0x310];
    CampingLookat lookats[1];          // +0x8e0; extent not established.
    unsigned char unknown900[0x1a2c - 0x900];
    float clock;                       // +0x1a2c
    int resetFlag;                     // +0x1a30; cleared by BotSetup.
};
extern CBotMan g_BotMan;
// The bot derives from the soldier object; only the members its functions touch are named.
class CBotObject {
  public:
    float GetClock();
    CBotObject *GetEnemyAI(unsigned int);
    struct EnemyInfo &GetEnemyInfo(unsigned int);
    struct EnemyInfo *GetpEnemyInfo(unsigned int);
    int EnemyInView();
    float GetHealth();
    char *GetName();
    unsigned char GetPlayerIndex();
    unsigned char GetTeam();
    float GetHealthPercent();
    void SetLookTarget(const CVector3 &);
    void SetMoveTarget(const CVector3 &);
    void LookAlongPath();
    int FollowingAStar();
    void StopAStar();
    int WithinGrenadeBlastRadius();
    void *GetCurrentPathNode();
    CampingPoint *GetCurrentCampingPoint();
    void SetPlayerIndex(unsigned char);
    void SetName(const char *);
    int CanFireWeapon();
    void StartReload();
    void FireAtEnemy();
    void Jump();
    void *GetGrenade();
    float GetMinWeaponRange();
    float GetMaxWeaponRange();
    void ClearEvents();
    void CheckTrigger();
    void BS_Enter_Dead();
    int BS_Dead();
    void BS_Enter_Goto();
    int BS_Goto();
    void BS_Enter_Combat();
    void BS_Enter_Retreat();
    void BS_Enter_Chase();
    int BS_Chase();
    int BS_Exit_UseMMG();
    void BS_Enter_Camp();
    void BS_Exit_Camp();
    int BS_Camp();
    void SetMoveState(eBotMoveState);
    int EnemyJustGoneOutOfSight();
    void SetNewObjectivePos();
    int ChangeState(eBotState, char *);
    int Respawn();
    void ClearBotData();
    void DropMPDumpObject();
    int RetreatForHealth();
    int RetreatForAmmo();
    int RetreatForWeapons();
    int CalcAStarPath(const CVector3 &, CAreaPathNode *);
    void SetCampingLookat();
    CVector3 EstimateEnemyPosition();
    int WantToRetreat();
    int BoredOfWaiting();
    int UpdateThreatDirections();
    int IsPointVisible(CVector3 *, CVector3 *);
    void AddAmmo(CWeapon *, int);
    void TeamAdjustDistance(float &, CVector3 &);
    void ChooseWeapon();
    void UpdateLookDir();
    int WantToStrafe();
    int DirectChase(CVector3 &);
    void RateWeapons();
    void ChangeWeapon(int);
    unsigned char unknown0000[0xd0 - 0x0];
    CMatrix matrix; // +0xd0; the first argument of CreateDumpObject
    unsigned char unknown0110[0x120 - 0x110];
    BSObject *scriptObject; // +0x120
    unsigned char unknown0124[0x198 - 0x124];
    float field198; // +0x198; set by BS_Dead
    unsigned char unknown019c[0xf20 - 0x19c];
    int currentWeaponIndex; // +0xf20
    unsigned char unknown0f24[0x15a8 - 0xf24];
    CAIObject *aiObject; // +0x15a8
    unsigned char unknown15ac[0x16d0 - 0x15ac];
    int eventCount; // +0x16d0
    int eventIndex; // +0x16d4
    unsigned char unknown16d8[0x16f0 - 0x16d8];
    CVector3 soundPosition; // +0x16f0; looked at while a sound is recent
    float soundTime; // +0x1700
    unsigned char unknown1704[0x1790 - 0x1704];
    EnemyInfo enemyInfo[1]; // +0x1790; extent not established
    unsigned char unknown17e0[0x1a10 - 0x17e0];
    EnemyInfo *enemyInView; // +0x1a10; null when no enemy is tracked
    unsigned char unknown1a14[0x1a20 - 0x1a14];
    CBotWeapon weapons[4]; // +0x1a20; four entries, from the loops over them
    CBotCharacter character; // +0x1ac0
    void *grenade; // +0x1b1c
    int bestWeaponIndex; // +0x1b20; compared with the current weapon by ChooseWeapon
    int fireBlockState; // +0x1b24; 5 stops CanFireWeapon
    int moveState; // +0x1b28; written by SetMoveState
    int objectiveType; // +0x1b2c
    unsigned char unknown1b30[0x1b40 - 0x1b30];
    CVector3 lookTarget; // +0x1b40
    CVector3 moveTarget; // +0x1b50
    unsigned char unknown1b60[0x1bd2 - 0x1b60];
    short pathStep; // +0x1bd2; -1 when a state starts a new path
    unsigned char unknown1bd4[0x1be0 - 0x1bd4];
    CVector3 threatPositions[5]; // +0x1be0; read by UpdateLookDir; extent from the count at +0x1c30
    int threatCount; // +0x1c30
    unsigned char unknown1c34[0x1c44 - 0x1c34];
    float combatTimer; // +0x1c44
    unsigned char unknown1c48[0x1c50 - 0x1c48];
    float fireDelay; // +0x1c50
    float reloadEndTime; // +0x1c54
    float respawnTime; // +0x1c58
    unsigned char unknown1c5c[0x1c68 - 0x1c5c];
    int fireAtEnemy; // +0x1c68
    int lookAlongPath; // +0x1c6c
    int jump; // +0x1c70
    unsigned char unknown1c74[0x1c7c - 0x1c74];
    int mmgIndex; // +0x1c7c; -1 for none
    int wantAmmo; // +0x1c80
    int lowAmmoWeaponType; // +0x1c84
    int wantHealth; // +0x1c88
    unsigned char unknown1c8c[0x1c90 - 0x1c8c];
    int wantWeapon; // +0x1c90
    unsigned char unknown1c94[0x1cb0 - 0x1c94];
    float grenadeDistanceSquared; // +0x1cb0
    unsigned char unknown1cb4[0x1cd4 - 0x1cb4];
    float campStartTime; // +0x1cd4
    float nextLookTime; // +0x1cd8
    int campingLookat; // +0x1cdc
    int currentCampingPoint; // +0x1ce0
    int crouching; // +0x1ce4; cleared with move state 2 and by BS_Exit_Camp
    int combatMode; // +0x1ce8; 8 on entering combat
    unsigned char unknown1cec[0x1e34 - 0x1cec];
    int enemyOutOfSightFrames; // +0x1e34; compared with OUT_OF_SIGHT_DELAY
    unsigned char unknown1e38[0x1e40 - 0x1e38];
    CVector3 teamCentre; // +0x1e40
    float boredDelay; // +0x1e50
    unsigned char unknown1e54[0x1e60 - 0x1e54];
    CVector3 retreatFrom; // +0x1e60
    unsigned char unknown1e70[0x1e8c - 0x1e70];
    float lookTargetExpireTime; // +0x1e8c
    unsigned char unknown1e90[0x1e94 - 0x1e90];
    unsigned char playerIndex; // +0x1e94
    char name[1]; // +0x1e95; extent not established
};
#endif
