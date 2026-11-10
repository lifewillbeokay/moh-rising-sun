// PackageHandler: common helpers for the shell's screens; see docs/Frontend.md.
#include "FEngObjects.h"
#include "GameSettings.h"
#include <stdio.h>
extern "C" int strcasecmp(const char *, const char *);
void DebugMsg(const char *, ...);

int PackageHandler::GetCurrentLanguage() { return 0; }
void PackageHandler::SetProfileNameOnScreen(int player) {
    SetObjectString(package, "Profile Name text", g_GameSettings.GetNameFromSettings(player));
}
void PackageHandler::Set2ProfileNamesOnScreen() {
    SetObjectString(package, "Profile Name text", g_GameSettings.GetNameFromSettings(0));
    SetObjectString(package, "Profile2 Name text", g_GameSettings.GetNameFromSettings(1));
}
int PackageHandler::SetGroupString(FEGroup *group, const char *name, char *text) {
    int result;
    FEStringObject *object = (FEStringObject *)group->FindChild(name);
    if (object) {
        object->text = text;
        object->changed = 1;
        result = 0;
    } else result = -3;
    return result;
}
int PackageHandler::SetGroupString(unsigned int groupHash, unsigned int hash, FEWideString *text) {
    FEStringObject *object;
    FEStringObject *group = (FEStringObject *)package->FindObjectByHash(groupHash);
    if (!group || group->type != 5 || (object = (FEStringObject *)((FEGroup *)group)->FindChild(hash)) == 0 || object->type != 2) return -1;
    object->text = text->text;
    object->changed = 1;
    return 0;
}
int PackageHandler::SetGroupString(unsigned int groupHash, unsigned int hash, const char *text) {
    FEStringObject *object;
    FEStringObject *group = (FEStringObject *)package->FindObjectByHash(groupHash);
    if (!group || group->type != 5 || (object = (FEStringObject *)((FEGroup *)group)->FindChild(hash)) == 0 || object->type != 2 || !text) return -1;
    object->text = text;
    object->changed = 1;
    return 0;
}
