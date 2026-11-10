// AI-assisted reconstruction from GR8E69. Fragments of the original PistolBehavior.cpp;
// method names come from the symbol table, member names are inferred.
#include "AIBehavior.h"
#include "ScriptTimers.h"
#include "StringCRC.h"
int ChooseSwordOrPistol(CAIObject *);

CPistolBehavior::CPistolBehavior() {
    highLevelState = (EAIHighLevelState)4;
    heuristicScore = 0.0f;
    weaponScript = 0;
}

CPistolBehavior::~CPistolBehavior() {}
float CPistolBehavior::GetCurrentHeuristicScore() { return heuristicScore; }
int CPistolBehavior::GetHeuristicUpdateFrequency() { return 60; }

int CPistolBehavior::UpdateHeuristicScore() {
    float score;
    if (weaponScript && ChooseSwordOrPistol(aiObject) == 0)
        score = 30.0f;
    else
        score = 0.0f;
    float previous = heuristicScore;
    heuristicScore = score;
    return previous != score;
}

int CPistolBehavior::StartBehavior() {
    BSObject *script = aiObject->filter->GetScriptObject();
    BSObjectTriggerEvent(weaponScript, 269, (void *)highLevelState, script, true);
    BSObjectTriggerEvent(script, 336, 0, weaponScript, true);
    return 1;
}

void CPistolBehavior::StopBehavior(bool immediate) {
    if (immediate)
        BSObjectTriggerEvent(weaponScript, 271, (void *)highLevelState, aiObject->filter->GetScriptObject(), true);
    else
        BSObjectTriggerEvent(weaponScript, 270, (void *)highLevelState, aiObject->filter->GetScriptObject(), true);
}

int CPistolBehavior::GetCapabilities(ECapability capability) {
    switch (capability) {
    case Capability1: return 1;
    case Capability2: return 1;
    case Capability3: return 1;
    case Capability5: return 0;
    case Capability6: return 1;
    case Capability7: return 1;
    default: return 0;
    }
}

unsigned int CPistolBehavior::GetCRC() { return GetStringCRC("Pistol"); }
void CPistolBehavior::SetWeaponScript(BSObject *script) { weaponScript = script; }
int CPistolBehavior::GetType() { return 6; }
BSObject *CPistolBehavior::GetScriptObject() { return weaponScript; }
