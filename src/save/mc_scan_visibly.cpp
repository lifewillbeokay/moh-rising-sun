// AI-assisted GR8E69 reconstruction; see docs/research/save-popups.md.
#include "MemoryCardFlow.h"
#include "Objectives.h"
extern "C" { int TIMER_gettick(); int TIMER_getfrequency(); }
void CMemoryCard::ScanVisibly(ScanTask &task, int index) {
    CMemoryCardPopup::Instance()->Push();
    CMemoryCardPopup::Instance()->displayed = 0; g_bIsBusy = 1;
    unsigned int count = GetDeviceCount();
    for (unsigned int i = index < 0 ? 0 : index; i < count; ++i) {
        const Device *device = GetDevice(i);
        if (device->IsAvailable()) {
            const unsigned char *text = (const unsigned char *)g_pPopUpMessageHandler->tables[0].GetString(1139);
            if (text) {
                unsigned short buffer[128], *out = buffer;
                while (*text) {
                    if (*text == '%') {
                        text += 2;
                        const unsigned short *name = (const unsigned short *)device->GetName();
                        while (*name) *out++ = *name++;
                    } else *out++ = *text++;
                }
                *out = 0;
                CMemoryCardPopup::Instance()->Display(buffer, 0, 0, 0, 0);
            }
            IssueTRCScan(task, device);
            int start = TIMER_gettick();
            while (CheckTRCScan() != 2) Wait();
            do { Wait(); } while ((TIMER_gettick() - start) * 1000 / TIMER_getfrequency() < 1000);
        }
        if (index >= 0) break;
    }
    g_bIsBusy = 0; CMemoryCardPopup::Instance()->Pop(); CancelReplyRequest();
}
