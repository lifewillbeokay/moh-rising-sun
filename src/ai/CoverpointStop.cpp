// AI-assisted reconstruction from GR8E69. Fragments of the original CoverpointBehavior.cpp;
// method names come from the symbol table, member names are inferred.
#include "SceneObjects.h"
#include "SceneNode.h"
#include "Weapon.h"
#include "AIBehavior.h"
#include "ScriptTimers.h"

void CCoverpointBehavior::StopBehavior(bool immediate) {
    if (immediate) {
        if (coverScript) {
            BSObjectTriggerEvent(coverScript, 271, (void *)aiObject->highLevelState, *(BSObject **)((char *)aiObject->filter + 8), true);
            LeaveCoverPoint();
        }
    } else {
        if (coverScript)
            BSObjectTriggerEvent(coverScript, 270, (void *)aiObject->highLevelState, *(BSObject **)((char *)aiObject->filter + 8), true);
        else
            BSObjectTriggerEvent(*(BSObject **)((char *)aiObject->filter + 8), 272, (void *)aiObject->highLevelState, 0, true);
        stopping = 1;
    }
}

void CCoverpointBehavior::ReleaseResources() {
    if (coverScript) {
        LeaveCoverPoint();
        stopping = 0;
    }
}
