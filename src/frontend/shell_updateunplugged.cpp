// ShellManager's short forwarding methods; see docs/Frontend.md.
#include "ShellManager.h"
#include <string.h>
#include "Shell.h"
extern CInputManager g_inputMgr;
struct BSObject;
void BSObjectTriggerEvent(BSObject *, unsigned short, void *, BSObject *, bool);
void DebugMsg(const char *, ...);
extern "C" int strcasecmp(const char *, const char *);

void ShellManager::UpdateUnplugged() {
    if (inMultiplayer && unknown_3bc8) DoMPControllerChecks();
    else if (inCoop && unknown_3bc8) DoCoopControllerChecks();
    else {
        CInputManager *input = &g_inputMgr;
        signed char none = -1;
        badPort = none;
        controllerBad = 0;
        if ((unsigned char)input->GetNumDevices() == 0) {
            badPort = none;
            controllerBad = 1;
            input->SetBadControllerPort(255);
        }
    }
}
void ShellManager::Draw() {
    engine->Render();
    DrawUnplugged();
}
void ShellManager::DrawUnplugged() {}
int ShellManager::GetPackageData(const char *name, unsigned char **data, bool &owned) { return loader->LoadPackage(name, data, owned); }
int ShellManager::LoadResources(FEPackage *package, long count, FEResourceRequest *requests) { return loader->LoadResources(package, count, requests); }
int ShellManager::UnloadResources(FEPackage *package, long count, FEResourceRequest *requests) { return loader->UnloadResources(package, count, requests); }
