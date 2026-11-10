// A fragment of the original ScriptedGag.cpp: the navigation point table and scripted gags, each a table
// of script actions registered under a name's CRC. Class and method names come from the symbol table;
// member names, the allocation scope and parameter names are inferred from this code.
#include <stdio.h>
#include "ScriptRuntime.h"
#include "ScriptTimers.h"
#include "AITargeting.h"
#include "AIDoodad.h"
#include "SceneObjects.h"
#include "StringCRC.h"
#include "ScriptedGag.h"

extern "C" const char *DWI_getallocid();
extern "C" void DWI_setallocid(const char *);
namespace AI { void Log(CAIObject *, char *, ...); }

// Names the allocations of a scope; the previous name comes back at its end.
class CAllocationScope {
public:
    CAllocationScope(const char *name) {
        previous = DWI_getallocid();
        DWI_setallocid(name);
    }
    ~CAllocationScope() { DWI_setallocid(previous); }
    const char *previous;
};

// File statics of the original, whose construction is not reconstructed.
extern ScriptActionMap g_NavPoints;
extern ScriptedGagMap g_mapGags;

void RegisterNavPoint(int id, BSObject *object) {
    CAllocationScope scope("RegisterNavPoint");
    ScriptActionMap::iterator it;
    it = g_NavPoints.find(id);
    char message[256];
    sprintf(message, "Tried to insert two NavPoints both with ID=%d\n", id);
    g_NavPoints[id] = object;
}

void UnregisterAllNavPoints() {
    g_NavPoints.clear();
}

BSObject *GetNavPoint(int id) {
    ScriptActionMap::iterator it;
    it = g_NavPoints.find(id);
    if (it == g_NavPoints.end())
        return 0;
    return it->second;
}

void CScriptedGag::RegisterGagAction(BSObject *object, const char *name, int action) {
    CAllocationScope scope("RegisterGagAction");
    unsigned int crc = GetStringCRC(name);
    CScriptedGag *gag;
    ScriptedGagMap::iterator it = g_mapGags.find(crc);
    if (it == g_mapGags.end())
        gag = g_mapGags[crc] = new ("source/ai_core/AI_side/ScriptedGag.cpp:90") CScriptedGag;
    else
        gag = it->second;
    gag->RegisterAction(object, action);
}

void CScriptedGag::UnregisterAllGagActions() {
    ScriptedGagMap::iterator it = g_mapGags.begin();
    ScriptedGagMap::iterator end = g_mapGags.end();
    for (; it != end; ++it)
        delete it->second;
    g_mapGags.clear();
}

void CScriptedGag::StartGag(BSObject *actor, const char *name, int action) {
    if (actor && actor->gameObject && actor->gameObject->GetSceneNode() && actor->gameObject->GetSceneNode()->GetAIObject())
        AI::Log(actor->gameObject->GetSceneNode()->GetAIObject(), "Gag %s Action %d\n", name, action);
    else
        AI::Log(0, "Gag %s Action %d\n", name, action);
    StartGag(actor, GetStringCRC(name), action);
}

void CScriptedGag::StartGag(BSObject *actor, unsigned int crc, int action) {
    ScriptedGagMap::iterator it = g_mapGags.find(crc);
    if (actor->gameObject && actor->gameObject->GetSceneNode() && actor->gameObject->GetSceneNode()->AsSoldierObject()) {
        CSoldierObject *soldier = actor->gameObject->GetSceneNode()->AsSoldierObject();
        soldier->SetCurrentGag(crc, action);
        ((CAISoldierObject *)((ISceneNode *)soldier)->GetAIObject())->activityZone.Disable();
    }
    it->second->StartAction(actor, action);
}

CScriptedGag::CScriptedGag() {}

CScriptedGag::~CScriptedGag() {}

void CScriptedGag::RegisterAction(BSObject *object, int id) {
    ScriptActionMap::iterator it;
    it = actions.find(id);
    char message[256];
    sprintf(message, "Tried to insert two Gag Actions both with Index=%d\n", id);
    actions[id] = object;
}

void CScriptedGag::StartAction(BSObject *object, int id) {
    ScriptActionMap::iterator it;
    it = actions.find(id);
    if (it == actions.end()) {
        BSObjectTriggerEvent(object, 316, 0, 0, true);
    } else {
        BSObjectTriggerEvent(it->second, 317, object, object, true);
        BSObjectTriggerEvent(object, 336, it->second, it->second, true);
    }
}
