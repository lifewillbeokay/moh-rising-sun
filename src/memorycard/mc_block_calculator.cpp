// AI-assisted reconstruction; see docs/research/card-tasks.md.
#include "RealmcBlockCalculator.h"
namespace Realmc {
void BlockCalculatorImp::Init(GCDriver *value, unsigned int maximum) {
    Clear();
    driver = value;
    limit = maximum;
}
void BlockCalculatorImp::Clear() {
    blocks = 0;
    files = 0;
    hasInfo = 0;
}
void BlockCalculatorImp::SetFileInfo(const CardID &card, const FileInfo &info) {
    blocks += driver->GetFileBlocks(card, &info, 0);
    files++;
    hasInfo = 1;
}
}
