// AI-assisted GR8E69 reconstruction; see docs/research/save-flow.md.
#include "MemoryCardFlow.h"
void CMemoryCard::Wait() {
    int minimal = g_bMinimalInteraction;
    if (minimal) THREAD_yield(1);
    else {
        if (g_bWaitingForReply && CMemoryCardPopup::Instance()->replied) {
            ProcessReply(CMemoryCardPopup::Instance()->GetReply());
            g_bWaitingForReply = minimal;
        }
        CMemoryCardPopup::Instance()->Wait();
        THREAD_yield(1);
    }
}
