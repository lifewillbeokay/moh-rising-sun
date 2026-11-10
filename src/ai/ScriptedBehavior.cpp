// AI-assisted reconstruction from GR8E69. Fragments of the original ScriptedBehavior.cpp;
// method names come from the symbol table, member names are inferred.
#include <string.h>
#include "AIBehavior.h"
#include "ScriptTimers.h"

CScriptedBehavior::CScriptedBehavior() {
    script = 0;
    pendingScript = 0;
    scriptActive = 0;
    scoreChanged = 0;
    scriptedType = 0;
    allowedStateMask = 0xffff;
    memset(capabilities, 0, sizeof(capabilities));
}

CScriptedBehavior::~CScriptedBehavior() {}

float CScriptedBehavior::GetCurrentHeuristicScore() {
    if (scriptActive) {
        if ((allowedStateMask >> aiObject->highLevelState) & 1)
            return 100.0f;
        return 0.0f;
    }
    return 0.0f;
}

int CScriptedBehavior::GetHeuristicUpdateFrequency() { return 10; }

int CScriptedBehavior::UpdateHeuristicScore() {
    if (scoreChanged) {
        scoreChanged = 0;
        return 1;
    }
    return 0;
}

int CScriptedBehavior::StartBehavior() {
    scoreChanged = 0;
    BSObject *self = aiObject->filter->GetScriptObject();
    BSObjectTriggerEvent(script, 269, (void *)aiObject->highLevelState, self, true);
    BSObjectTriggerEvent(self, 336, 0, script, true);
    return 1;
}

void CScriptedBehavior::StopBehavior(bool immediate) {
    BSObject *current = script;
    scoreChanged = 0;
    if (current) {
        if (immediate)
            BSObjectTriggerEvent(current, 271, 0, aiObject->filter->GetScriptObject(), true);
        else
            BSObjectTriggerEvent(current, 270, 0, aiObject->filter->GetScriptObject(), true);
    }
    if (pendingScript) {
        script = pendingScript;
        pendingScript = 0;
    }
}

int CScriptedBehavior::GetCapabilities(ECapability capability) { return capabilities[capability]; }
void CScriptedBehavior::ReleaseResources() {}

void CScriptedBehavior::StartScript(BSObject *newScript) {
    memset(capabilities, 0, sizeof(capabilities));
    scriptedType = 0;
    if (script)
        pendingScript = newScript;
    else
        script = newScript;
    scriptActive = 1;
    scoreChanged = 1;
}

void CScriptedBehavior::StopScript() {
    scriptActive = 0;
    scoreChanged = 1;
    allowedStateMask = 0xffff;
}

void CScriptedBehavior::SetCapability(ECapability capability, bool value) { capabilities[capability] = value; }

void CScriptedBehavior::SetAllowedHighLevelState(EAIHighLevelState state) {
    if (allowedStateMask == 0xffff)
        allowedStateMask = 0;
    allowedStateMask |= 1 << state;
}

int CScriptedBehavior::GetType() { return 5; }
BSObject *CScriptedBehavior::GetScriptObject() { return script; }
