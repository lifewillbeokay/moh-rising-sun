// AI-assisted reconstruction from GR8E69. Fragments of the original GrenadeResponseBehavior.cpp;
// method names come from the symbol table, member names are inferred.
#include "AIBehavior.h"
#include "ScriptTimers.h"
void DestroyBSObject(BSObject *);

void CGrenadeResponseBehavior::StopBehavior(bool immediate) {
    if (responseScript) {
        if (immediate)
            BSObjectTriggerEvent(responseScript, 271, 0, aiObject->scriptObject, true);
        else
            BSObjectTriggerEvent(responseScript, 270, 0, aiObject->scriptObject, true);
    }
}

int CGrenadeResponseBehavior::GetCapabilities(ECapability capability) { return capability == Capability1; }

void CGrenadeResponseBehavior::ReleaseResources() {
    DestroyBSObject(responseScript);
    responseScript = 0;
}
