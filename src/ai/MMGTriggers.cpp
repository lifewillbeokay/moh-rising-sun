// AI-assisted reconstruction of the mounted machine gun's registered guns; see docs/Behaviors.md.
// The map tags and names come from the symbol table.
#include "profile.h"
#include "AIBehavior.h"
#include <map>
extern "C" void *DWI_alloc(const char *, int, int);
extern "C" {
const char *DWI_getallocid();
void DWI_setallocid(const char *);
}
#include "ScriptRuntime.h"
inline void *operator new(unsigned int size, const char *label) { return DWI_alloc(label, size, 1024); }
struct MMGBidInfo {
    float bid;
    CAIObject *bidder;
    MMGBidInfo() : bid(0.0f), bidder(0) {}
};
typedef _STL::map<TriggerObject *, MMGBidInfo *> BidInfoMap;
extern BidInfoMap g_MMGBidInfoMap;
extern _STL::map<int, BSObject *> g_MMGFeederMap;
extern int g_pNumTriggers;
extern TriggerObject g_pTriggerObjects[];
void BSObjectTriggerEvent(BSObject *, unsigned short, void *, BSObject *, bool);

// Saves and restores the allocator's tag around a scope; the class name is inferred.
class DwiAllocIdScope {
  public:
    DwiAllocIdScope(const char *id) {
        saved = DWI_getallocid();
        DWI_setallocid(id);
    }
    ~DwiAllocIdScope() { DWI_setallocid(saved); }
    const char *saved;
};

void CMountedMachineGunBehavior::RegisterMMGTriggers() {
    DwiAllocIdScope scope("RegisterMMGTriggers");
    for (TriggerObject *trigger = g_pTriggerObjects; trigger != g_pTriggerObjects + g_pNumTriggers; trigger++) {
        if (trigger->flagBits & 0x10000000) {
            MMGBidInfo *info = new ("source/ai_core/AI_side/MountedMachineGunBehavior.cpp:773") MMGBidInfo;
            g_MMGBidInfoMap[trigger] = info;
        }
    }
}

void CMountedMachineGunBehavior::DeregisterMMGTriggers() {
    BidInfoMap::iterator it(0);
    for (it = g_MMGBidInfoMap.begin(); it != g_MMGBidInfoMap.end(); ++it)
        delete (*it).second;
    g_MMGBidInfoMap.clear();
}

void CMountedMachineGunBehavior::RegisterFeeder(int linkCode, BSObject *feeder) {
    DwiAllocIdScope scope("RegisterFeeder");
    _STL::map<int, BSObject *>::iterator found(0);
    found = g_MMGFeederMap.find(linkCode);
    g_MMGFeederMap[linkCode] = feeder;
}

void CMountedMachineGunBehavior::DeregisterAllFeeders() { g_MMGFeederMap.clear(); }

int CMountedMachineGunBehavior::GetFeederByLinkCode(int linkCode) {
    _STL::map<int, BSObject *>::iterator found(0);
    found = g_MMGFeederMap.find(linkCode);
    return found == g_MMGFeederMap.end() ? 0 : (int)(*found).second;
}

void CMountedMachineGunBehavior::ForceToMMGFeeder() {
    BidInfoMap::iterator it(0);
    float closest = 25.0f;
    BSObject *nearest = 0;
    for (it = g_MMGBidInfoMap.begin(); it != g_MMGBidInfoMap.end(); it++) {
        TriggerObject *trigger = (*it).first;
        CVector3 position;
        trigger->GetPosition(position);
        float distance = aiObject->position.Distance(position);
        if (!(distance >= closest)) {
            nearest = trigger->scriptObject;
            closest = distance;
        }
    }
    if (nearest)
        BSObjectTriggerEvent(nearest, 348, aiObject->filter->GetScriptObject(), 0, true);
}
