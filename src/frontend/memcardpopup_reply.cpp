// CMemoryCardPopup's reply handling; see docs/Frontend.md.
#include "SaveGamePopups.h"
void CMemoryCardPopup::RequestReply() {
    waitingForReply = 1;
    unsigned long mask = g_Shell.GetCombinedJoyPadActiveMask();
    if (mask) {
        engine->activeMask = mask;
    }
    ShowSelections();
}
int CMemoryCardPopup::GetReply() {
    waitingForReply = 0;
    replied = 0;
    return reply;
}
