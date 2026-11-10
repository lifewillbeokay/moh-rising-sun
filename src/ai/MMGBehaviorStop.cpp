// AI-assisted reconstruction from GR8E69. Fragments of the original MountedMachineGunBehavior.cpp;
// method names come from the symbol table, member names are inferred.
#include "ScriptRuntime.h"
#include "AIBehavior.h"
#include "ScriptTimers.h"
#include "TriggerObject.h"

void CMountedMachineGunBehavior::StopBehavior(bool immediate) {
    BSObject *script = gunScript;
    if (script) {
        if (immediate)
            BSObjectTriggerEvent(script, 271, (void *)behaviorStyle, aiObject->filter->GetScriptObject(), true);
        else {
            BSObjectTriggerEvent(script, 270, (void *)behaviorStyle, aiObject->filter->GetScriptObject(), true);
            stopping = 1;
        }
    }
}

void CMountedMachineGunBehavior::ReleaseResources() {
    if (aiObject->GetNextBehaviorType() != 3 && gunScript) {
        MarkMGAsUsed(gunScript->nativeObject, false);
        gunScript = 0;
        arrived = 0;
    }
    stopping = 0;
}

void CMountedMachineGunBehavior::SetBehaviorStyle(EAIHighLevelState style) { behaviorStyle = style; }

int CMountedMachineGunBehavior::DoIOwnMMG(TriggerObject *trigger) {
    if (gunScript) {
        if (gunScript->nativeObject == trigger)
            return 1;
    }
    return 0;
}
