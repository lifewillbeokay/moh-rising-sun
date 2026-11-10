// AI-assisted reconstruction of the memory-card library's GameCube interface: small accessors; see docs/Memory.md.
#include "RealmcInterface.h"
namespace Realmc {
int GCInterface::IsBusy() {
    return mTaskManager.current ? 1 : 0;
}
int GCInterface::GetBlockSize(const CardID &card) {
    return mpDriver->GetSectorSize(card);
}
BlockCalculatorImp *GCInterface::GetBlockCalculator() {
    return &mBlockCalculator;
}
} // namespace Realmc
