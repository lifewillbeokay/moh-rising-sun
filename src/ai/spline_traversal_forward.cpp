// AI-assisted reconstruction from GR8E69; see docs/Paths.md.
#include "AISplinePath.h"
#include "ScriptTimers.h"
void CAISplinePathTraversal::GetNextPointForward(const CVector3 &position, const CVector3 &direction, CVector3 *out, BSObject *object, CAISplinePathModule *, CAIAreaPathFinding *finder) {
    bool crossed = true;
    if (state == 7) {
        CVector3 point(position);
        point += direction;
        *out = point;
        if (object) BSObjectTriggerEvent(object, 53, 0, 0, true);
        return;
    }
    if (state == 5 || state == 6) {
        GetFinalPointForward(&nextPoint.position);
        if (nextPoint.WasCrossedDistance(position, &previousDistanceSquared)) {
            state = 7;
            if (object) BSObjectTriggerEvent(object, 53, 0, 0, true);
        }
    } else {
        if (state == 1 || state == 2) crossed = nextPoint.WasCrossedDistance(position, &previousDistanceSquared);
        if (crossed) {
            float t = GetCurrentParameterValue(parameter, position);
            if (!(t < 0.0f)) parameter = t;
            if (!(parameter < 1.0f)) {
                if (segmentIndex >= path->lastSegment - 1) {
                    segmentIndex = path->lastSegment - 1;
                    segment = &path->segments[segmentIndex];
                    previousDistanceSquared = 3.4028234663852886e38f;
                    if (state == 2 || state == 4) state = 6;
                    else if (state == 1 || state == 3) state = 5;
                    GetFinalPointForward(&nextPoint.position);
                } else {
                    if (object) BSObjectTriggerEvent(object, 163, 0, 0, true);
                    ++segmentIndex;
                    ++segment;
                    SetupNextSegmentForward(position);
                }
            } else {
                if (state == 1) {
                    if (object) BSObjectTriggerEvent(object, 52, 0, 0, true);
                    state = 3;
                } else if (state == 2) {
                    if (object) BSObjectTriggerEvent(object, 52, 0, 0, true);
                    state = 4;
                }
                segment->Expand(parameter, nextPoint.position);
                MakeModifiedNextPoint();
            }
        }
    }
    if (state == 2) {
        finder->UpdateAStarPathWalk();
        *out = finder->locomotion->GetCurrentObjectivePosition();
    } else {
        out->Set(nextPoint.position.x, nextPoint.position.y, nextPoint.position.z);
    }
}
