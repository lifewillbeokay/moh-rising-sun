// AI-assisted reconstruction of the coverpoint registry; see docs/Behaviors.md. Names come from the symbol table.
#include "profile.h"
#include "AIBehavior.h"
#include "TriggerObject.h"
#include <list>
#include <algorithm>
extern "C" {
const char *DWI_getallocid();
void DWI_setallocid(const char *);
}
typedef _STL::list<CCoverPoint *> CoverPointList;
extern CoverPointList g_Coverpoints;
extern CoverPointList::iterator g_CoverpointsOutOfAreaIterator;
extern int g_CoverpointsOutOfAreaValid;

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

void CCoverpointBehavior::ResetCoverpoints() {
    g_Coverpoints.clear();
    g_CoverpointsOutOfAreaValid = 0;
}

int CCoverpointBehavior::RegisterCoverpoint(TriggerObject *trigger) {
    DwiAllocIdScope scope("RegisterCoverpoint");
    CCoverPoint *point = (CCoverPoint *)trigger;
    CoverPointList::iterator found;
    found = _STL::find(g_Coverpoints.begin(), g_Coverpoints.end(), point);
    point->InitFlags();
    if (found == g_Coverpoints.end()) {
        g_CoverpointsOutOfAreaValid = 0;
        g_Coverpoints.push_back(point);
        return 1;
    }
    return 0;
}

void CCoverpointBehavior::InvalidateCoverPointByID(int id) {
    if (!g_CoverpointsOutOfAreaValid) {
        g_CoverpointsOutOfAreaIterator = g_Coverpoints.begin();
        g_CoverpointsOutOfAreaValid = 1;
    }
    CoverPointList::iterator it;
    for (it = g_Coverpoints.begin(); it != g_Coverpoints.end(); ++it) {
        CCoverPoint *point = *it;
        if (((TriggerObject *)point)->GetID() == id)
            point->Invalidate();
    }
}
