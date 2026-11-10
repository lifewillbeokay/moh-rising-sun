// AI-assisted reconstruction from GR8E69. Fragments of the original PolemineBehavior.cpp;
// method names come from the symbol table, member names are inferred.
#include "AIBehavior.h"
#include "ScriptTimers.h"
#include "StringCRC.h"

CPolemineBehavior::CPolemineBehavior() {
    heuristicScore = 0.0f;
    weaponScript = 0;
}

CPolemineBehavior::~CPolemineBehavior() {}
float CPolemineBehavior::GetCurrentHeuristicScore() { return heuristicScore; }
int CPolemineBehavior::GetHeuristicUpdateFrequency() { return 60; }

int CPolemineBehavior::UpdateHeuristicScore() {
    float score;
    if (weaponScript)
        score = 100.0f;
    else
        score = 0.0f;
    float previous = heuristicScore;
    heuristicScore = score;
    return previous != score;
}

int CPolemineBehavior::StartBehavior() {
    BSObject *script = aiObject->filter->GetScriptObject();
    BSObjectTriggerEvent(weaponScript, 269, 0, script, true);
    BSObjectTriggerEvent(script, 336, 0, weaponScript, true);
    return 1;
}

void CPolemineBehavior::StopBehavior(bool immediate) {
    if (immediate)
        BSObjectTriggerEvent(weaponScript, 271, 0, aiObject->filter->GetScriptObject(), true);
    else
        BSObjectTriggerEvent(weaponScript, 270, 0, aiObject->filter->GetScriptObject(), true);
}

int CPolemineBehavior::GetCapabilities(ECapability capability) { return capability >= Capability0 && capability <= Capability3; }

unsigned int CPolemineBehavior::GetCRC() { return GetStringCRC("Polemine"); }
void CPolemineBehavior::SetWeaponScript(BSObject *script) { weaponScript = script; }
int CPolemineBehavior::GetType() { return 7; }
BSObject *CPolemineBehavior::GetScriptObject() { return weaponScript; }
