// AI-assisted GR8E69 reconstruction; see docs/research/save-flow.md.
#include "MemoryCardFlow.h"
#include "MohLoader.h"
extern "C" void LOCALE_setstate(void *, int, int);
extern int wasAlreadyHere asm("wasAlreadyHere.4236");
#ifdef DECOMP_COMPARE
int CMemoryCard::OnStartup() {
    MohLoader *loader = MohLoader::Instance();
    if (g_GameSettings.skuType == 1) {
        if (loader->gameDataValid != 1) {
            CSaveGameLanguageScreen::Instance()->SelectLanguage();
            if (g_GameSettings.profile->language != 0)
                g_GameSettings.profile->profiles[g_GameSettings.profile->profileIndex[0]].languageSelected = 1;
        }
        int mainLanguage, miscLanguage;
        switch (g_GameSettings.profile->language) {
        case 3: mainLanguage = 2; miscLanguage = 2; break;
        case 2: mainLanguage = 3; miscLanguage = 1; break;
        case 6: mainLanguage = 4; miscLanguage = 3; break;
        case 7: mainLanguage = 1; miscLanguage = 4; break;
        default: mainLanguage = 0; miscLanguage = 0; break;
        }
        LOCALE_setstate(g_pTextDatabase, 0, mainLanguage);
        LOCALE_setstate(g_pMiscTextDatabase, 0, miscLanguage);
    }
    if (!wasAlreadyHere) {
        g_bMinimalInteraction = MohLoader::Instance()->gameDataValid;
        CMemoryCardPopup::Instance()->Push(); IssueStart();
        int state;
        do { state = CheckStart(); Wait(); } while ((unsigned int)(state - 2) > 1);
        g_bMinimalInteraction = 0; CMemoryCardPopup::Instance()->Pop();
        g_bMinimalInteraction = 1; Device::Update(true); g_bMinimalInteraction = 0;
        if (state != 2) { CancelReplyRequest(); return 0; }
        CancelReplyRequest();
    }
    return 1;
}
#endif
