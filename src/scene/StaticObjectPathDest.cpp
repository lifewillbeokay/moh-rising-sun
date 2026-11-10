// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original static_obj.cpp; member names are descriptive.
#include "SceneObjects.h"
#include "AISplinePath.h"
#include "GameSettings.h"

extern "C" float sqrtf(float);

static inline void SetSum(CVector3 &r, const CVector3 &a, const CVector3 &b) {
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    r.z = a.z + b.z;
}

static inline int Mission() { return g_GameSettings.profile->saveData.mission; }
static inline int Level() { return g_GameSettings.profile->saveData.level; }

static inline void SetSub(CVector3 &r, const CVector3 &a, const CVector3 &b) {
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    r.z = a.z - b.z;
}

void CStaticObject::UpdatePathMotionDestAndSpeed() {
    CVector3 here;
    CVector3 target;
    here = pathPosition;
    switch ((int)pathState) {
    case 5:
    case 6:
    default:
        if (!(pathSpeed > 0.0f)) {
            if (pathState == 6)
                pathState = 1;
            else
                pathState = 0;
            pathSpeed = 0.0f;
            pathMaxSpeed = 0.0f;
            flags.scriptMoving = 0;
        }
    case 2:
    case 3:
    {
        CVector3 forward;
        ((ISceneNode *)this)->GetForward(forward);
        ((CAISplinePathTraversal *)pathTraversal)->GetNextPointForward(here, forward, &target, 0, 0, 0);
        CVector3 final;
        ((CAISplinePathTraversal *)pathTraversal)->GetFinalPointForward(&final);
        float distance = here.DistanceSquared(final);
        if (!(distance >= 400.0f)) {
            float stop = pathSpeed / (pathAcceleration + pathAcceleration);
            stop = stop * stop * 0.001f;
            if (((CAISplinePathTraversal *)pathTraversal)->state == 7 || !(distance >= stop * stop)) {
                pathState = 4;
                here.DistanceSquared(target);
                ((CAISplinePathTraversal *)pathTraversal)->GetFinalPointForward(&target);
            }
        }
    }
        pathDest = target;
        break;
    case 4: {
        ((CAISplinePathTraversal *)pathTraversal)->GetFinalPointForward(&target);
        if (!(here.DistanceSquared(target) >= 4.0f)) {
            TriggerScriptEvent(53, 0, 0);
            pathSpeed = 0.0f;
            pathMaxSpeed = 0.0f;
            ((CAISplinePathTraversal *)pathTraversal)->path = 0;
            pathState = 0;
            flags.scriptMoving = 0;
        }
        pathDest = target;
        break;
    }
    }
    switch ((int)pathState) {
    case 2:
        break;
    case 3: {
        CVector3 leader, toLeader, direction;
        ((ISceneNode *)pathFollowTarget)->GetPosition(leader);
        SetSub(toLeader, leader, here);
        SetSub(direction, target, here);
        float along = direction.x * toLeader.x + direction.y * toLeader.y + direction.z * toLeader.z;
        float length = sqrtf(direction.LengthSquared());
        if (length != 0.0f)
            direction *= 1.0f / length;
        direction *= along;
        SetSum(leader, here, direction);
        float gap = leader.DistanceSquared(here);
        if (!(along <= 0.0f))
            gap = gap + pathFollowDistanceSquared;
        else
            gap = pathFollowDistanceSquared - gap;
        if (Mission() == 4 && Level() == 2) {
            if (!(gap <= 25.0f))
                pathMaxSpeed = pathField104;
            else if (!(gap >= -25.0f))
                pathMaxSpeed = pathField104 * -0.6f;
            else
                pathMaxSpeed = pathField104 * 0.2f;
        } else {
            if (!(gap <= 25.0f))
                pathMaxSpeed = pathField104;
            else if (gap < -25.0f)
                pathMaxSpeed = 0.0f;
            else
                pathMaxSpeed = pathField104 * 0.6f;
        }
        break;
    }
    case 4:
        pathMaxSpeed = 0.0100100115f;
        break;
    case 5:
    case 6:
        pathMaxSpeed = 0.0f;
        break;
    }
}
