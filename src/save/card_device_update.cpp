// AI-assisted reconstruction from GR8E69; see docs/research/broad-source-sweep.md.
#include "MemoryCardDevice.h"
extern int gs_bChangeOfState;
extern int gs_statusBefore[2], gs_statusNow[2];
extern "C" int CARDProbeEx(int, int *, int *);
int CMemoryCard::Device::ChangeOfState() {
    int changed = gs_bChangeOfState;
    gs_bChangeOfState = 0;
    return changed;
}
void CMemoryCard::Device::Update(bool refresh) {
    if (g_hMemoryCard->IsBusy())
        return;
    if (gs_statusNow[0] != -1)
        gs_statusBefore[0] = gs_statusNow[0];
    if (gs_statusNow[1] != -1)
        gs_statusBefore[1] = gs_statusNow[1];
    gs_statusNow[0] = CARDProbeEx(0, 0, 0);
    gs_statusNow[1] = CARDProbeEx(1, 0, 0);
    if (refresh) {
        if (gs_statusNow[0] != -1)
            gs_statusBefore[0] = gs_statusNow[0];
        if (gs_statusNow[1] != -1)
            gs_statusBefore[1] = gs_statusNow[1];
        gs_cards[0].Cache(false);
        gs_cards[1].Cache(false);
    } else {
        bool changed0 = gs_statusNow[0] != gs_statusBefore[0] && gs_statusNow[0] != -1;
        bool changed1 = gs_statusNow[1] != gs_statusBefore[1] && gs_statusNow[1] != -1;
        gs_bChangeOfState = changed0 || changed1;
        if (changed0)
            gs_cards[0].Cache(true);
        if (changed1)
            gs_cards[1].Cache(true);
    }
}
