// AI-assisted reconstruction of the sound buss free list; see docs/Sound.md.
#include "SoundManager.h"
#include "SoundBuss.h"
#include "PlacementNew.h"
extern cSoundBuss g_AllBusses[32];
extern cSoundBuss *g_pFreeBussList;
void cSoundBussMan::InitSoundBussFreeList() {
    g_pFreeBussList = g_AllBusses;
    cSoundBuss *buss = g_AllBusses;
    for (int i = 31; i != 0; i--) {
        buss->next = buss + 1;
        buss++;
    }
    buss->next = 0;
}
void ShutdownSoundBussMan() {
    g_pFreeBussList = 0;
}
cSoundBuss *GetFreeSoundBuss() {
    cSoundBuss *buss = 0;
    if (g_pFreeBussList) {
        buss = g_pFreeBussList;
        g_pFreeBussList = buss->next;
    }
    if (buss)
        buss = new (buss) cSoundBuss;
    return buss;
}
