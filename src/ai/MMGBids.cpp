// AI-assisted reconstruction of the mounted machine gun's registered guns; see docs/Behaviors.md.
// The map tags and names come from the symbol table.
#include "profile.h"
#include "AIBehavior.h"
#include <map>
extern "C" {
const char *DWI_getallocid();
void DWI_setallocid(const char *);
}
#include "ScriptRuntime.h"
struct MMGBidInfo {
    float bid;
    CAIObject *bidder;
};
typedef _STL::map<TriggerObject *, MMGBidInfo *> BidInfoMap;
extern BidInfoMap g_MMGBidInfoMap;
extern _STL::map<int, BSObject *> g_MMGFeederMap;
extern int g_pNumTriggers;
extern TriggerObject g_pTriggerObjects[];
void *DWI_alloc(const char *, int, int);
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

float GetBestBid(TriggerObject *trigger) {
    BidInfoMap::iterator it(0);
    it = g_MMGBidInfoMap.find(trigger);
    return (*it).second->bid;
}

CAIObject *GetBestBidder(TriggerObject *trigger) {
    BidInfoMap::iterator it(0);
    it = g_MMGBidInfoMap.find(trigger);
    return (*it).second->bidder;
}

float CalculateHeuristicForMMG(TriggerObject *, CAIObject *);

bool CanClaimMMG(TriggerObject *trigger, CAIObject *object) {
    if (GetBestBidder(trigger) == object)
        return true;
    float best = GetBestBid(trigger);
    return CalculateHeuristicForMMG(trigger, object) > best;
}

void BidOnMMG(TriggerObject *trigger, CAIObject *object, float bid) {
    if (trigger && object) {
        BidInfoMap::iterator it(0);
        it = g_MMGBidInfoMap.find(trigger);
        MMGBidInfo *info = (*it).second;
        if (!(bid <= info->bid)) {
            info->bidder = object;
        } else if (info->bidder != object) {
            return;
        }
        info->bid = bid;
    }
}
