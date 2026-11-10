// AI-assisted reconstruction from GR8E69. Part of the original Soldier_object.cpp; there is no
// Frontline counterpart. The lip-sync callbacks are static: each takes the audio filter's state.
#include "SceneObjects.h"
#include "WorldObject.h"
#include "Compartment.h"
#include "Volume.h"
#include "BSGameObject.h"
#include "ScriptRuntime.h"
#include "EAGLTransform.h"
#include "../game-containers/TargetTypes.h"
void DebugMsg(const char *, ...);
int LUAEvaluateState(void *, const char *, int, const char *);

extern WeakPtr<CSoldierObject, 8> g_lipSyncSoldier[32];
extern int g_nextLipSyncSoldier;

// The filter state's field at +0x1c holds its slot in g_lipSyncSoldier.
struct LipSyncFilterState {
    unsigned char unknown_0[0x1c];
    unsigned int slot; // +0x1c
};

// The soldier's choreographer client at +0x38; the slots' names are the symbols in its table.
class SoldierChoreographerClient {
  public:
    virtual void MoveTo(const COORD3 &, float);
    virtual void *GetSkeleton();
    virtual COORD3 GetPosition();
    virtual float GetRotation();
};
// Where the current gag is recorded in the script's data; offsets from this use, names inferred.
struct GagScriptData {
    unsigned char unknown_0[0x30];
    int gag;     // +0x30
    int gagType; // +0x34
};

void CSoldierObject::SaveGagState() {
    GagScriptData *data = (GagScriptData *)((BSObject *)((ISceneNode *)this)->GetScriptObject())->gameObject->GetScriptData();
    data->gag = currentGag;
    data->gagType = currentGagType;
    SoldierChoreographerClient *client = (SoldierChoreographerClient *)((char *)this + 0x38);
    *(COORD3 *)savedGagState = client->GetPosition();
    savedGagTime = client->GetRotation();
}

int CSoldierObject::IsInAnyOtherCompartment(int current) {
    CWorldObject::CompartmentIterator it = g_scene.world->BeginCompartments();
    CWorldObject::CompartmentIterator end = g_scene.world->EndCompartments();
    for (; it != CWorldObject::CompartmentIterator(end); ++it) {
        if ((*it)->index != current) {
            CCollision collision;
            collision.first = *it;
            collision.second = this;
            if (collision.Test() == 2)
                return 1;
        }
    }
    return 0;
}

int CSoldierObject::EvaluateScript(const char *name, unsigned int value, const char *arg) {
    return LUAEvaluateState(this, name, value, arg);
}

void CSoldierObject::LipSyncFilterInit(void *filter, int soldier, int channel) {
    if (channel) {
        DebugMsg("<!!!> LIPSYNC: Ignoring channel %d.", channel);
    } else if (filter) {
        LipSyncFilterState *state = (LipSyncFilterState *)filter;
        state->slot = g_nextLipSyncSoldier++;
        if (g_nextLipSyncSoldier > 31)
            g_nextLipSyncSoldier = 0;
        g_lipSyncSoldier[state->slot].Assign((ISubject *)soldier);
    }
}

void CSoldierObject::LipSyncFilterRestore(void *filter) {
    if (filter) {
        unsigned int slot = ((LipSyncFilterState *)filter)->slot;
        if (slot <= 31)
            g_lipSyncSoldier[slot].Assign(0);
    }
}
