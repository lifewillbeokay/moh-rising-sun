// AI-assisted reconstruction from GR8E69; see docs/Targeting.md.
#ifndef GAME_AI_TARGETING_H
#define GAME_AI_TARGETING_H
#include "AIMovement.h"
class CAIObjectParameters;
class ISceneNode;
// Observed script-object and position prefix, not a complete allocation,
// inheritance model or virtual interface. Unknown bytes retain their ownership.
class CAIObject {
public:
    unsigned char unknown00[0x0c];
    BSObject *scriptObject;
    unsigned char unknown10[0x20];
    CVector3 position;
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
    bool HasTarget() const { return aiObject || scriptObject; }
};
typedef char CAITargetStorageSizeCheck[sizeof(CAITarget) == 12 ? 1 : -1];

// Observed prefix through the modified-position vector at +0x70. This view
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
    unsigned char unknown3c[0x34];
    CVector3 modifiedPosition;
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
    float GetDistanceToTargetSquaredXYZReal();
    BSObject *GetTargetScript() const;
    bool IsNonAITarget() const;
    bool HasValidTarget() const;
    bool GetTargetGuessPosition(CVector3 &);
};
#endif
