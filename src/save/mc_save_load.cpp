// AI-assisted GR8E69 reconstruction; see docs/research/save-flow.md.
#include "MemoryCardFlow.h"
#ifdef DECOMP_COMPARE
extern void GenerateScratchpadCRC();
int CMemoryCard::Save(SPlayerProfile &profile) {
    CMemoryCardPopup::Instance()->Push();
    g_bIsBusy = 1;
    if (profile.device == 0xbad) {
        profile.device = 0;
        for (unsigned int i = 0; i < (unsigned int)GetDeviceCount(); ++i) {
            if (GetDevice(i)->IsAvailable()) { profile.device = i; break; }
        }
    }
    int retry = 1;
    memcpy(g_pScratchpad, &profile, sizeof(profile));
    GenerateScratchpadCRC();
    do {
        IssueSave(profile.name, profile.device);
        bool done;
        do {
            int state = CheckSave();
            if (state == 2) { retry = 0; done = true; }
            else if (state == 4) { retry = 1; done = true; }
            else { done = false; Wait(); }
        } while (!done);
    } while (retry);
    g_bIsBusy = retry;
    CMemoryCardPopup::Instance()->Pop();
    CancelReplyRequest();
    if (!SaveSuccessful()) return DeclinedToSave() ? -2 : -1;
    return WasDataWritten() ? 1 : 2;
}
int CMemoryCard::Load(const char *name, unsigned int device, SPlayerProfile &profile) {
    CMemoryCardPopup::Instance()->Push();
    int retry = 1;
    g_bIsBusy = 1;
    do {
        IssueLoad(name, device);
        bool done;
        do {
            int state = CheckLoad();
            if (state == 2) { retry = 0; done = true; }
            else if (state == 4) { retry = 1; done = true; }
            else { done = false; Wait(); }
        } while (!done);
    } while (retry);
    if (LoadSuccessful()) {
        int language = g_GameSettings.profile->language;
        memcpy(&profile, g_pScratchpad, sizeof(profile));
        g_GameSettings.Set_Language((ELanguages)language);
        profile.device = device;
    }
    CMemoryCardPopup::Instance()->Pop();
    g_bIsBusy = retry;
    CancelReplyRequest();
    return LoadSuccessful();
}
#endif
