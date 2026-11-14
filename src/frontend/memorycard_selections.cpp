// AI-assisted reconstruction from GR8E69; see docs/research/frontend-popup-list.md.
#include "SaveGamePopups.h"
extern unsigned long optionEvents[4] asm("events.3428");
extern int optionGuard asm("_.tmp_1.3429");
extern unsigned long enableInputs asm("event.3430");
extern int enableGuard asm("_.tmp_2.3431");
extern unsigned long labelZero asm("event.3432");
extern int labelGuard asm("_.tmp_3.3433");
extern unsigned long disableInputs asm("event.3437");
extern int disableGuard asm("_.tmp_4.3438");
#ifdef DECOMP_COMPARE
void CMemoryCardPopup::ShowSelections() {
    if (!optionGuard) {
        unsigned long a = FEHashUpper("Enable Option 0");
        unsigned long b = FEHashUpper("Enable Option 1");
        unsigned long c = FEHashUpper("Enable Option 2");
        optionEvents[3] = FEHashUpper("Enable Option 3");
        optionGuard = 1;
        optionEvents[0] = a; optionEvents[1] = b; optionEvents[2] = c;
    }
    for (unsigned int i = 0; i < 4; ++i)
        if (selectable[i]) engine->QueueMessage(optionEvents[i], 0, 0, 0, 0);
    if (!enableGuard) { enableInputs = FEHashUpper("Enable_All_Inputs"); enableGuard = 1; }
    engine->QueueMessage(enableInputs, 0, 0, 0, 0);
    if (selectable[0] && !selectable[1] && !selectable[2] && !selectable[3]) {
        if (!labelGuard) { labelZero = FEHashUpper("Label0"); labelGuard = 1; }
        engine->QueueMessage(labelZero, 0, 0, 0, 0);
    }
}
void CMemoryCardPopup::HideSelections() {
    if (!disableGuard) { disableInputs = FEHashUpper("Disable_All_Inputs"); disableGuard = 1; }
    engine->QueueMessage(disableInputs, 0, 0, 0, 0);
}
#endif
