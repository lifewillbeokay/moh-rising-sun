// AI-assisted reconstruction from GR8E69. Fragments of the original NullWeaponBehavior.cpp;
// method names come from the symbol table, member names are inferred.
#include "AIBehavior.h"
#include "ScriptTimers.h"
#include "StringCRC.h"

CNullWeaponBehavior::CNullWeaponBehavior() {
    highLevelState = (EAIHighLevelState)4;
    heuristicScore = 0.0f;
    weaponScript = 0;
}

CNullWeaponBehavior::~CNullWeaponBehavior() {}
float CNullWeaponBehavior::GetCurrentHeuristicScore() { return heuristicScore; }
int CNullWeaponBehavior::GetHeuristicUpdateFrequency() { return 60; }

int CNullWeaponBehavior::UpdateHeuristicScore() {
    float score;
    if (weaponScript)
        score = 30.0f;
    else
        score = 0.0f;
    float previous = heuristicScore;
    heuristicScore = score;
    return previous != score;
}

int CNullWeaponBehavior::StartBehavior() {
    BSObject *script = aiObject->filter->GetScriptObject();
    BSObjectTriggerEvent(weaponScript, 269, (void *)highLevelState, script, true);
    BSObjectTriggerEvent(script, 336, 0, weaponScript, true);
    return 1;
}

void CNullWeaponBehavior::StopBehavior(bool immediate) {
    if (immediate)
        BSObjectTriggerEvent(weaponScript, 271, (void *)highLevelState, aiObject->filter->GetScriptObject(), true);
    else
        BSObjectTriggerEvent(weaponScript, 270, (void *)highLevelState, aiObject->filter->GetScriptObject(), true);
}

int CNullWeaponBehavior::GetCapabilities(ECapability capability) { return 0; }

unsigned int CNullWeaponBehavior::GetCRC() { return GetStringCRC("NullWeapon"); }
void CNullWeaponBehavior::SetWeaponScript(BSObject *script) { weaponScript = script; }
int CNullWeaponBehavior::GetType() { return 9; }
BSObject *CNullWeaponBehavior::GetScriptObject() { return weaponScript; }
