// ShellManager's short forwarding methods; see docs/Frontend.md.
#include "ShellManager.h"
#include <string.h>
#include "Shell.h"
extern CInputManager g_inputMgr;
struct BSObject;
void BSObjectTriggerEvent(BSObject *, unsigned short, void *, BSObject *, bool);
void DebugMsg(const char *, ...);
extern "C" int strcasecmp(const char *, const char *);

void ShellManager::PlayMovie(EMovieType type, int index, bool flag) { PlayMovie(movies.GetMoviePathsStruct(type, index), flag); }
FENode *ShellManager::GetPauseScreen() {
    FENode *result = 0;
    FEList *list = &engine->packages;
    if (list) result = list->FindNode("Pause.fng");
    return result;
}
void ShellManager::SetNumberJoyPads(unsigned char count) {
    if (engine) {
        if (count) engine->SetNumJoyPads(count);
        else engine->ResetJoyPad();
    }
}
int ShellManager::LocalizePackage(FEPackage *package) {
    LocalizeObjects((FEObject *)package->objectList.head);
    return 0;
}
