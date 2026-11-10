// ShellManager's short forwarding methods; see docs/Frontend.md.
#include "ShellManager.h"
#include <string.h>
#include "Shell.h"
extern CInputManager g_inputMgr;
struct BSObject;
void BSObjectTriggerEvent(BSObject *, unsigned short, void *, BSObject *, bool);
void DebugMsg(const char *, ...);
extern "C" int strcasecmp(const char *, const char *);

int ShellManager::HasScreen(const char *name) {
    int found = 0;
    FEPackage *next;
    for (FEPackage *p = (FEPackage *)engine->packages.head; p; p = next) {
        next = (FEPackage *)p->next;
        if (strcasecmp(p->fileName, name) == 0) {
            found = 1;
            break;
        }
    }
    return found;
}
void ShellManager::InitMovies() {
    CShellMovieSettings *settings = &movies;
    if (!settings->loaded) {
        CConfigFile config("data\\game.cfg");
        settings->Load(config);
    }
}
