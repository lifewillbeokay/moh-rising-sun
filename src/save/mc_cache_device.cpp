// AI-assisted GR8E69 reconstruction; see docs/research/save-flow.md.
#include "MemoryCardFlow.h"
void CMemoryCard::Cache(Device *device) {
    CMemoryCardPopup::Instance()->Push();
    CMemoryCardPopup::Instance()->displayed = 0;
    g_bIsBusy = 1;
    device->IssueCache();
    while (device->CheckCache() != 2) Wait();
    g_bIsBusy = 0;
    CMemoryCardPopup::Instance()->Pop();
    CancelReplyRequest();
}
