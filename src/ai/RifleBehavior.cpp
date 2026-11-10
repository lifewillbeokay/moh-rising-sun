// AI-assisted reconstruction from GR8E69. Fragments of the original RifleBehavior.cpp;
// method names come from the symbol table, member names are inferred.
#include "AIBehavior.h"
#include "ScriptTimers.h"
#include "StringCRC.h"

CRifleBehavior::CRifleBehavior() {
    highLevelState = (EAIHighLevelState)4;
    heuristicScore = 0.0f;
    weaponScript = 0;
}

CRifleBehavior::~CRifleBehavior() {}
float CRifleBehavior::GetCurrentHeuristicScore() { return heuristicScore; }
int CRifleBehavior::GetHeuristicUpdateFrequency() { return 60; }

int CRifleBehavior::UpdateHeuristicScore() {
    float score;
    if (weaponScript)
        score = 30.0f;
    else
        score = 0.0f;
    float previous = heuristicScore;
    heuristicScore = score;
    return previous != score;
}

int CRifleBehavior::StartBehavior() {
    BSObject *script = aiObject->filter->GetScriptObject();
    BSObjectTriggerEvent(weaponScript, 269, (void *)highLevelState, script, true);
    BSObjectTriggerEvent(script, 336, 0, weaponScript, true);
    return 1;
}

void CRifleBehavior::StopBehavior(bool immediate) {
    if (immediate)
        BSObjectTriggerEvent(weaponScript, 271, (void *)highLevelState, aiObject->filter->GetScriptObject(), true);
    else
        BSObjectTriggerEvent(weaponScript, 270, (void *)highLevelState, aiObject->filter->GetScriptObject(), true);
}

int CRifleBehavior::GetCapabilities(ECapability capability) {
    if (capability == Capability0 || capability == Capability1 || capability == Capability6 || capability == Capability7)
        return 1;
    return 0;
}

unsigned int CRifleBehavior::GetCRC() { return GetStringCRC("Rifle"); }
void CRifleBehavior::SetWeaponScript(BSObject *script) { weaponScript = script; }
int CRifleBehavior::GetType() { return 7; }
BSObject *CRifleBehavior::GetScriptObject() { return weaponScript; }
