// AI-assisted reconstruction from GR8E69; see docs/research/frontend-popup-list.md.
#include "PackageHandlers.h"
#include "FengInterface.h"
extern "C" void MEM_free(void *);
char *TLT_LoadFileNormal(const char *, int *, int);
void TLT_ForceLoadFileNormalLocked(bool);
extern unsigned int g_uInstanceCount;
extern char *g_pAssets;
extern const char *popupAssetFilename asm("filename.7674");
inline void *operator new(unsigned int size, const char *label) { return DWI_alloc(label, size, 1024); }
CSaveGamePopup::CSaveGamePopup(const char *name) {
    displayed = 1;
    if (g_uInstanceCount == 0) g_pAssets = TLT_LoadFileNormal(popupAssetFilename, 0, 0);
    engine = new ("source/SaveGame/SaveGameCore.cpp:103") FEngine;
    interface = new ("source/SaveGame/SaveGameCore.cpp:106") CFengInterface(engine);
    engine->game = interface;
    engine->SetExecution(true);
    engine->SetNumJoyPads(4);
    interface->SetBigFilePtr(g_pAssets);
    interface->RegisterUIObject(this);
    package = engine->PushPackage(name, 0, 255);
    TLT_ForceLoadFileNormalLocked(true);
    sprite = new ("source/SaveGame/SaveGameCore.cpp:129") CSprite("data\\loading\\medal.gsh", (CSprite::ESpriteLoadType)2);
    unknown_14 = 0;
    TLT_ForceLoadFileNormalLocked(false);
    ++g_uInstanceCount;
}
CSaveGamePopup::~CSaveGamePopup() {
    engine->UnloadPackage(package);
    delete interface;
    delete engine;
    if (--g_uInstanceCount == 0) {
        MEM_free(g_pAssets);
        g_pAssets = 0;
    }
    if (sprite) { delete sprite; sprite = 0; }
}
