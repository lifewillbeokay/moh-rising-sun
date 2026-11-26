// AI-assisted GR8E69 reconstruction; see docs/research/save-popups.md.
#include "MemoryCardGC.h"
extern "C" { void MEM_clear(void *, unsigned int); void MEM_copy(void *, const void *, unsigned int); }
// Caller-side views. Slot names follow the BlockCalculatorImp symbols; no object is constructed.
class CardBlockCalculatorView {
public:
    virtual void Clear(); virtual int GetResult();
    virtual void SetFileInfo(const Realmc::CardID &, const CardFileInfoView &);
};
struct CardStartInfoView {
    CardFileInfoView file;
    int blockCount; // +0x30
    Realmc::CardID card; // +0x34; zeroed with the record.
    int unknown38, unknown3c, requiredFiles, requiredBlocks, unknown48;
};
typedef char CardStartInfoSizeCheck[sizeof(CardStartInfoView) == 76 ? 1 : -1];
#ifdef DECOMP_COMPARE
void CMemoryCardGC::IssueStart() {
    MohLoader::Instance()->resetEnabled = 0;
    Init(); operationActive = 1;
    CardStartInfoView &start = *(CardStartInfoView *)startInfo;
    MEM_clear(&start, 76); MEM_copy(&start, &fileInfo, 48);
    Realmc::CardID card = {0, 0};
    start.unknown38 = start.unknown48 = start.unknown3c = 0;
    CardBlockCalculatorView *calculator = (CardBlockCalculatorView *)interface->GetBlockCalculator();
    calculator->Clear(); calculator->SetFileInfo(card, fileInfo);
    int blocks = calculator->GetResult();
    start.requiredFiles = 1; start.requiredBlocks = blocks; start.blockCount = blocks;
    interface->TrcStartGame(&start); TIMER_elapsedticks();
}
#endif
