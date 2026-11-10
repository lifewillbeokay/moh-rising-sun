// HandlerMPGameGlobalOptions' setup; see docs/Frontend.md.
#include "PackageHandlers.h"
#include "GameSettings.h"
int HandlerMPGameGlobalOptions::Init(const char *name) {
    SPlayerProfile *profile = g_GameSettings.GetProfile(0);
    FEStringObject *object = (FEStringObject *)package->FindObjectByName("ProfileName");
    object->text = profile->name;
    object->flags |= 0x400000;
    character = profile->unknown_c4e5;
    SetObjectListIndex("Character", character + 1);
    SetObjectListIndex("CharPic", character + 1);
    HandlerMPGameOptions::Init(name);
    return 0;
}
