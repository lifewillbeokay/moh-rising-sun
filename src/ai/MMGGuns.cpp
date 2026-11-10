// AI-assisted reconstruction of the mounted machine gun's registered guns; see docs/Behaviors.md.
// The map tags and names come from the symbol table.
#include "profile.h"
#include "AIBehavior.h"
#include <map>
extern "C" {
const char *DWI_getallocid();
void DWI_setallocid(const char *);
}
extern _STL::map<int, CStaticObject *> g_guns;
extern _STL::map<int, BSObject *> g_MMGFeederMap;

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

void CMountedMachineGunBehavior::RegisterGun(int linkCode, CStaticObject *gun) {
    DwiAllocIdScope scope("RegisterGun");
    _STL::map<int, CStaticObject *>::iterator found(0);
    found = g_guns.find(linkCode);
    g_guns[linkCode] = gun;
}

void CMountedMachineGunBehavior::DeregisterGun(int linkCode) {
    _STL::map<int, CStaticObject *>::iterator found(0);
    found = g_guns.find(linkCode);
    if (found != g_guns.end())
        g_guns.erase(found);
}

void CMountedMachineGunBehavior::DeregisterAllGuns() { g_guns.clear(); }

CStaticObject *CMountedMachineGunBehavior::GetGunByLinkCode(int linkCode) {
    _STL::map<int, CStaticObject *>::iterator found(0);
    found = g_guns.find(linkCode);
    return found == g_guns.end() ? 0 : (*found).second;
}
