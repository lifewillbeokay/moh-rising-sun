// AI-assisted GR8E69 reconstruction; see docs/research/frontend-screens.md.
#ifdef DECOMP_COMPARE
#include "PackageHandlers.h"
#include "Shell.h"
#include "GameSettings.h"
#include <stdio.h>
extern unsigned long keyHash asm("sKeyPressed.3279");
extern int keyGuard asm("_.tmp_0.3280");
extern unsigned long sendHash asm("sSend.3281");
extern int sendGuard asm("_.tmp_1.3282");
extern unsigned long backHash asm("sBack.3283");
extern int backGuard asm("_.tmp_2.3284");
extern unsigned long setupHash asm("sSetup.3285");
extern int setupGuard asm("_.tmp_3.3286");
int HandlerVirtualKeyboard::HandleEvent(unsigned int event, FEObject *object, unsigned long) {
    if (!keyGuard) { keyHash = FEHashUpper("Key Pressed"); keyGuard = 1; }
    if (!sendGuard) { sendHash = FEHashUpper("SEND"); sendGuard = 1; }
    if (!backGuard) { backHash = FEHashUpper("BACKSPACE"); backGuard = 1; }
    if (!setupGuard) { setupHash = FEHashUpper("Text Entry Setup"); setupGuard = 1; }
    char *text = GetMyString();
    if (event == FEHashUpper("Clear String Field")) ClearStringField();
    else if (event == keyHash) {
        // The concrete keyboard handlers share this observed +0x50 cursor field.
        int &cursor = *(int *)((char *)this + 0x50);
        if (!cursor) { ClearStringField(); cursor = 1; }
        if (object->hash == backHash) RemovePreviousLetter(text);
        else AddLetterToString(text, object->name[0]);
    } else if (event != setupHash && event == FEHashUpper("Clear Profile Name")) *text = 0;
    bool ended = false; char letter[2] = {0, 0};
    for (int i = 0; i < GetStringLength(); ++i) {
        if (ended || !text[i]) { ended = true; letter[0] = ' '; }
        else letter[0] = text[i];
        char name[16]; sprintf(name, "Letter%d", i);
        SetObjectString(package, name, letter);
    }
    return 0;
}
#endif
