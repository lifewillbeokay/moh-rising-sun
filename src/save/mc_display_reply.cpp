// AI-assisted GR8E69 reconstruction; see docs/research/save-flow.md.
#include "MemoryCardFlow.h"
void CMemoryCard::Display(const unsigned short *message, const unsigned short *a, const unsigned short *b, const unsigned short *c, const unsigned short *d) {
    CMemoryCardPopup::Instance()->Display(message, a, b, c, d);
}
void CMemoryCard::RequestReply() {
    if (g_bMinimalInteraction) ProcessReply(DefaultReply());
    else {
        CMemoryCardPopup::Instance()->RequestReply();
        g_bWaitingForReply = 1;
    }
}
