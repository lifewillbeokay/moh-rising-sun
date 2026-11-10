// AI-assisted reconstruction from GR8E69. Fragments of the original SwordBehavior.cpp;
// method names come from the symbol table, member names are inferred.
#include "AIBehavior.h"
#include "ScriptTimers.h"
#include "StringCRC.h"
int ChooseSwordOrPistol(CAIObject *);

CSwordBehavior::CSwordBehavior() {
    heuristicScore = 0.0f;
    swordDrawn = 0;
    weaponScript = 0;
}

CSwordBehavior::~CSwordBehavior() {}
float CSwordBehavior::GetCurrentHeuristicScore() { return heuristicScore; }
int CSwordBehavior::GetHeuristicUpdateFrequency() { return 60; }

int CSwordBehavior::UpdateHeuristicScore() {
    float score;
    if (weaponScript && ChooseSwordOrPistol(aiObject) == 1)
        score = 30.0f;
    else
        score = 0.0f;
    float previous = heuristicScore;
    heuristicScore = score;
    return previous != score;
}

int CSwordBehavior::StartBehavior() {
    BSObject *script = aiObject->filter->GetScriptObject();
    BSObjectTriggerEvent(weaponScript, 269, (void *)aiObject->highLevelState, script, true);
    BSObjectTriggerEvent(script, 336, 0, weaponScript, true);
    return 1;
}

void CSwordBehavior::StopBehavior(bool immediate) {
    if (immediate)
        BSObjectTriggerEvent(weaponScript, 271, (void *)aiObject->highLevelState, aiObject->filter->GetScriptObject(), true);
    else
        BSObjectTriggerEvent(weaponScript, 270, (void *)aiObject->highLevelState, aiObject->filter->GetScriptObject(), true);
}

int CSwordBehavior::GetCapabilities(ECapability capability) {
    if (capability == Capability0)
        return 1;
    if (capability == Capability2 || capability == Capability3 || capability == Capability6)
        return 1;
    return 0;
}

unsigned int CSwordBehavior::GetCRC() { return GetStringCRC("Sword"); }
void CSwordBehavior::SetWeaponScript(BSObject *script) { weaponScript = script; }
int CSwordBehavior::IsSwordDrawn() { return swordDrawn; }
void CSwordBehavior::SetSwordDrawn(bool drawn) { swordDrawn = drawn; }

int ChooseSwordOrPistol(CAIObject *ai) {
    int choice = 0;
    CBehavior *sword = ai->FindMatchingBehavior(BehaviorTypeSword);
    CBehavior *pistol = ai->FindMatchingBehavior(BehaviorTypePistol);
    BSObject *swordScript = sword->GetScriptObject();
    BSObject *pistolScript = pistol->GetScriptObject();
    if (sword->enabled && swordScript && (!pistolScript || !pistol->enabled)) {
        choice = 1;
    } else if (pistol->enabled && pistolScript && (!swordScript || !sword->enabled)) {
        choice = 0;
    } else {
        // The original tests the embedded module's address for null.
        if (&ai->targeting) {
            if (ai->targeting.GetTarget().HasTarget()) {
                if (!(ai->targeting.GetDistanceToTargetSquaredXYZReal() >= 100.0f))
                    choice = 1;
            }
        }
    }
    return choice;
}

int CSwordBehavior::GetType() { return 8; }
BSObject *CSwordBehavior::GetScriptObject() { return weaponScript; }
