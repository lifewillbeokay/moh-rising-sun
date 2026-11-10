// AI-assisted reconstruction from GR8E69. Fragments of the original BuddyFollowBehavior.cpp;
// method and static names come from the symbol table, other member names are inferred.
#include "SceneNode.h"
#include "AIBehavior.h"
#include "AIFilterGlobal.h"
#include "ScriptTimers.h"

BSObject *CBuddyFollowBehavior::m_pBuddyFollowObject = 0;
float CBuddyFollowBehavior::m_defaultStopDistance = 0.0f;
float CBuddyFollowBehavior::m_defaultMoveThreshold = 0.0f;
float CBuddyFollowBehavior::m_defaultBreakOffAttackDistance = 0.0f;
float CBuddyFollowBehavior::m_defaultStopAttackDistance = 0.0f;

CBuddyFollowBehavior::CBuddyFollowBehavior(CAIActivityZone *zone) {
    activityZone = zone;
    teleportArea = -1;
    heuristicScore = 0.0f;
    stopped = 0;
    movingAwayExpireTime = 0.0f;
    distancesSet = 0;
    teleportAreaCount = 0;
    movingAway = 0;
    enabled = 0;
    cantReachTarget = 0;
}

CBuddyFollowBehavior::~CBuddyFollowBehavior() {}

void CBuddyFollowBehavior::Enable() {
    enabled = 1;
    if (!distancesSet) {
        stopDistance = m_defaultStopDistance;
        moveThreshold = m_defaultMoveThreshold;
        breakOffAttackDistance = m_defaultBreakOffAttackDistance;
        stopAttackDistance = m_defaultStopAttackDistance;
    }
}

void CBuddyFollowBehavior::Disable() {
    enabled = 0;
    activityZone->Disable();
}

float CBuddyFollowBehavior::GetCurrentHeuristicScore() { return heuristicScore; }
int CBuddyFollowBehavior::GetHeuristicUpdateFrequency() { return 10; }

int CBuddyFollowBehavior::UpdateHeuristicScore() {
    float score = 0.0f;
    float distance = UpdateDistanceToPlayer();
    int changed = 0;
    if (m_pBuddyFollowObject) {
        if (cantReachTarget && !(cantReachExpireTime >= g_aigAIFilterGlobalObject.time))
            cantReachTarget = 0;
        if (movingAway && !(movingAwayExpireTime >= g_aigAIFilterGlobalObject.time))
            movingAway = 0;
        if (!cantReachTarget) {
            if (movingAway) {
                score = 90.0f;
            } else {
                float threshold = stopped ? GetStartDistance() : GetStopDistance();
                if (!(distance <= threshold))
                    score = 90.0f;
            }
        }
    }
    if (aiObject->highLevelState == 0)
        activityZone->Disable();
    else
        activityZone->UseMovingTarget((ISceneNode *)g_scene.GetPlayer(0), GetStartDistance());
    if (score != heuristicScore)
        changed = 1;
    heuristicScore = score;
    return changed;
}

int CBuddyFollowBehavior::StartBehavior() {
    stopped = 0;
    BSObject *script = aiObject->scriptObject;
    BSObjectTriggerEvent(m_pBuddyFollowObject, 269, (void *)movingAway, script, true);
    BSObjectTriggerEvent(script, 336, 0, m_pBuddyFollowObject, true);
    return 1;
}

void CBuddyFollowBehavior::StopBehavior(bool immediate) {
    stopped = 1;
    BSObject *script = aiObject->scriptObject;
    if (immediate)
        BSObjectTriggerEvent(m_pBuddyFollowObject, 271, 0, script, true);
    else
        BSObjectTriggerEvent(m_pBuddyFollowObject, 270, 0, script, true);
}

int CBuddyFollowBehavior::GetCapabilities(ECapability capability) {
    return capability >= Capability0 && capability <= Capability1;
}

void CBuddyFollowBehavior::CantReachTarget() {
    movingAway = 0;
    cantReachTarget = 1;
    heuristicScore = 0.0f;
    cantReachExpireTime = g_aigAIFilterGlobalObject.time + 1.0f;
    BSObjectTriggerEvent(aiObject->scriptObject, 268, 0, 0, true);
}

void CBuddyFollowBehavior::RegisterBuddyFollowObject(BSObject *object, float stop, float threshold, float breakOff, float stopAttack) {
    m_pBuddyFollowObject = object;
    m_defaultStopDistance = stop;
    m_defaultMoveThreshold = threshold;
    m_defaultBreakOffAttackDistance = breakOff;
    m_defaultStopAttackDistance = stopAttack;
}

void CBuddyFollowBehavior::SetDistances(float stop, float threshold, float breakOff, float stopAttack) {
    stopDistance = stop;
    moveThreshold = threshold;
    breakOffAttackDistance = breakOff;
    stopAttackDistance = stopAttack;
    distancesSet = 1;
}

float CBuddyFollowBehavior::UpdateDistanceToPlayer() {
    return aiObject->GetDistanceXYReal(((ISceneNode *)g_scene.GetPlayer(0))->GetAIObject());
}

void CBuddyFollowBehavior::EnterBuddyTeleportArea(int area) {
    teleportArea = area;
    teleportAreaCount++;
}

void CBuddyFollowBehavior::ExitBuddyTeleportArea(int area) {
    if (teleportAreaCount > 0) {
        if (--teleportAreaCount == 0)
            teleportArea = -1;
    }
}

int CBuddyFollowBehavior::IsBuddyInArea(int area) { return area == teleportArea; }

float CBuddyFollowBehavior::GetStopDistance() {
    return aiObject->highLevelState != 4 ? stopDistance : stopAttackDistance;
}

float CBuddyFollowBehavior::GetStartDistance() {
    return aiObject->highLevelState != 4 ? moveThreshold : breakOffAttackDistance;
}

void CBuddyFollowBehavior::BumpPlayer() {
    movingAwayExpireTime = g_aigAIFilterGlobalObject.time + 6.0f;
    if (!movingAway) {
        movingAway = 1;
        if (enabled)
            BSObjectTriggerEvent(aiObject->scriptObject, 268, 0, 0, true);
    }
}

void CBuddyFollowBehavior::FinishedMovingAway() {
    movingAway = 0;
    movingAwayExpireTime = 0.0f;
    BSObjectTriggerEvent(aiObject->scriptObject, 268, 0, 0, true);
}

int CBuddyFollowBehavior::GetType() { return 2; }
