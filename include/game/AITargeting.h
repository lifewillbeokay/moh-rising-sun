// AI-assisted reconstruction from GR8E69; see docs/Targeting.md.
#ifndef GAME_AI_TARGETING_H
#define GAME_AI_TARGETING_H
#include "AIMovement.h"
// Original enum tag; numeric members are descriptive, not recovered names.
enum AIFILTER_WEAPON_AIM_MODE {
    WeaponAimModeUnset = 0, WeaponAimMode1 = 1, WeaponAimMode2 = 2,
    WeaponAimMode6 = 6, WeaponAimMode8 = 8
};
class CAIObjectParameters;
class ISceneNode;
// Observed script-object, position and forward-vector prefix, not a complete allocation,
// inheritance model or virtual interface. Unknown bytes retain their ownership.
class CAIObject {
public:
    unsigned char unknown00[0x0c];
    BSObject *scriptObject;
    unsigned char unknown10[0x20];
    CVector3 position;
    unsigned char unknown40[0x20];
    CVector3 forward;
};
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
    unsigned char unknown80[0x78];
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
    void AdjustAimPosition(CVector3 *, int, AIFILTER_WEAPON_AIM_MODE *);
    void ChooseSniperAimMode(AIFILTER_WEAPON_AIM_MODE *);
    void ChooseBazookaAimMode(AIFILTER_WEAPON_AIM_MODE *);
    void GetAdjustedAimTargetPosition(CVector3 &);
    bool DoBlindFire();
    float GetDistanceToTargetSquaredXYZReal();
    BSObject *GetTargetScript() const;
    bool IsNonAITarget() const;
    bool HasValidTarget() const;
    bool GetTargetGuessPosition(CVector3 &);
};
typedef char AITargetingPrefixCheck[sizeof(CAITargeting) == 0x108 ? 1 : -1];
#endif
