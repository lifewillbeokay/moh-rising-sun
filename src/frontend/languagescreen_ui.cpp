// AI-assisted GR8E69 reconstruction; see docs/research/save-popups.md.
#include "MemoryCardFlow.h"
extern "C" { void LOCALE_setstate(void *, int, int); const unsigned short *LOCALE_getstr(void *, int); }
extern int g_eResponse;
extern unsigned long selected asm("uLANGUAGE_SELECTED.3365");
extern int selectedGuard asm("_.tmp_1.3366");
extern unsigned long selectText asm("uSELECTLANGUAGETEXT.3367");
extern int selectTextGuard asm("_.tmp_2.3368");
extern unsigned long update asm("uUPDATESCREEN.3369");
extern int updateGuard asm("_.tmp_3.3370");
extern unsigned long confirm asm("uCONFIRM.3371");
extern int confirmGuard asm("_.tmp_4.3372");
#ifdef DECOMP_COMPARE
void CSaveGameLanguageScreen::OnUIEvent(unsigned int event, FEObject *object) {
    idleFrames = 0;
    if (!selectedGuard) { selected = FEHashUpper("Language Selected"); selectedGuard = 1; }
    if (!selectTextGuard) { selectText = FEHashUpper("Select Language text"); selectTextGuard = 1; }
    if (!updateGuard) { update = FEHashUpper("Update Screen"); updateGuard = 1; }
    if (!confirmGuard) { confirm = FEHashUpper("Confirm text"); confirmGuard = 1; }
    if (event == selected) {
        int i = 0; while (i < 5 && buttonHashes[i] != object->hash) ++i;
        g_GameSettings.Set_Language((ELanguages)languages[i]); g_eResponse = 1;
    } else if (event == update) {
        int i = 0; while (i < 5 && buttonHashes[i] != object->hash) ++i;
        LOCALE_setstate(g_pMiscTextDatabase, 0, GetLocaleID(i));
        FEString *string = (FEString *)package->FindObjectByHash(selectText);
        SetString(string, LOCALE_getstr(g_pMiscTextDatabase, 1), false);
        string = (FEString *)package->FindObjectByHash(confirm);
        SetString(string, LOCALE_getstr(g_pMiscTextDatabase, 2), false);
        for (i = 0; i < 5; ++i) {
            string = (FEString *)package->FindObjectByHash(buttonHashes[i]);
            SetString(string, LOCALE_getstr(g_pMiscTextDatabase, GetButtonName(i)), false);
        }
    }
}
#endif
