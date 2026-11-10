// AI-assisted reconstruction of CAITactics, which scores positions around an AI object; see docs/Behaviors.md.
// Method names come from the symbol table; the score terms' names are inferred.
#include "AITargeting.h"
#include "BSGameObject.h"
#include "ScriptRuntime.h"
extern "C" float sqrtf(float);
// The script data's two fields these scores read. Names are inferred.
struct TacticsScriptData {
    unsigned char unknown_00[64];
    int aggression;         // +0x40; 2 favors closing on the target, 1 scores nothing.
    unsigned char unknown_44[96 - 68];
    float engagementRange;  // +0x60
};


float CAITactics::GetTacticalPositionScore(const CVector3 &point, float range) {
    if (!AIPathFinding::IsPointOnNetwork(point))
        return 0.0f;
    CVector3 targetPosition = targeting->GetTarget().GetPosition();
    TacticsScriptData *data = (TacticsScriptData *)owner->scriptObject->gameObject->GetScriptData();
    float distanceSquared = point.DistanceSquared(targetPosition);
    if (distanceSquared > data->engagementRange * data->engagementRange || !(distanceSquared >= 4.0f))
        return 0.0f;
    CVector3 look, side;
    ((CAIObjectInterface *)owner)->GetLookDirection(look, side);
    float length = sqrtf(look.LengthSquared());
    if (length != 0.0f)
        look *= 1.0f / length;
    if (!owner->IsObjectInFieldOfView(point, look, targetPosition, owner->fieldOfViewObjectRange,
                                      owner->fieldOfViewObjectAngle, false, 0))
        return 0.0f;
    float aggression = AggressionScore(point, range);
    float allies = AlliesScore(point, range);
    float angle = AngleFromTargetScore(point, range);
    return aggression + allies + angle;
}

float CAITactics::AggressionScore(const CVector3 &point, float range) {
    CVector3 targetPosition = targeting->GetTarget().GetPosition();
    TacticsScriptData *data = (TacticsScriptData *)owner->scriptObject->gameObject->GetScriptData();
    float toTarget = point.Distance(targetPosition);
    float score = physics->position.Distance(targetPosition);
    score -= toTarget;
    score /= range;
    score += 1.0f;
    score *= 0.5f;
    float result = 0.0f;
    if (data->aggression == 2)
        result = score * 10.0f;
    else if (data->aggression != 1)
        result = (1.0f - score) * 10.0f;
    return result;
}

float CAITactics::AlliesScore(const CVector3 &point, float) {
    float score = 200.0f;
    const float near = 1.0f;
    for (CAIObject *other = CAIObject::GetGlobalListStart(); other; other = other->nextGlobal) {
        if (other != owner && other->team == owner->team) {
            float distance = point.Distance(other->position);
            if (!(distance >= 10.0f)) {
                if (!(distance > near))
                    return 0.0f;
                score -= (near - (distance - near) * 0.11111111f) * 20.0f;
                if (!(score > 0.0f)) {
                    score = 0.0f;
                    break;
                }
            }
        }
    }
    return score;
}

float CAITactics::AngleFromTargetScore(const CVector3 &point, float) {
    CAITarget target = targeting->GetTarget();
    CVector3 toPoint, targetPosition, forward;
    targetPosition = target.GetPosition();
    forward = target.GetForward();
    toPoint = point;
    toPoint -= targetPosition;
    float length = sqrtf(toPoint.LengthSquared());
    if (length != 0.0f)
        toPoint *= 1.0f / length;
    length = sqrtf(forward.LengthSquared());
    if (length != 0.0f)
        forward *= 1.0f / length;
    return (-(toPoint.x * forward.x + toPoint.y * forward.y + toPoint.z * forward.z) + 1.0f) * 50.0f;
}
