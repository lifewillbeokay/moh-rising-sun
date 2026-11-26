// AI-assisted reconstruction; see docs/research/card-tasks.md.
#include "RealmcDriver.h"
extern "C" void THREAD_yield(int);
namespace Realmc {
#ifdef DECOMP_COMPARE
int GCDriver::NGCSportsBioCreate(GcFileDescriptor *file) {
    int result;
    do {
        result = CARDCreate(file->card.slot, file->info.name, file->info.size, &file->cardFile);
        THREAD_yield(1);
    } while (result == CARD_RESULT_BUSY);
    switch (result) {
    case CARD_RESULT_READY: file->status = 0; file->onCard = 0; break;
    case CARD_RESULT_FATAL_ERROR: file->status = -1; break;
    case CARD_RESULT_NOCARD: file->status = -2; break;
    case CARD_RESULT_EXIST: file->status = -4; break;
    case CARD_RESULT_NOENT: file->status = -5; break;
    case CARD_RESULT_INSSPACE: file->status = -6; break;
    case CARD_RESULT_NAMETOOLONG: file->status = -7; break;
    case CARD_RESULT_IOERROR: file->status = -8; break;
    default: file->status = -10; break;
    }
    return result;
}
int GCDriver::VerifyHeaderChecksum(GcFileDescriptor *file) {
    int status = 15, sum = 0;
    int remaining = file->headerSize - 4;
    int count = MEMCARD_SECTOR_SIZE;
    while (remaining > 0) {
        if (remaining < MEMCARD_SECTOR_SIZE) count = remaining;
        status = ReadFile((OpenFileDescriptor *)file, 0, count, 0);
        if (status) break;
        remaining -= count;
        for (int i = 0; i < count; i++) sum += ((signed char *)buffer)[i];
    }
    if (!status) {
        int expected = 0;
        status = ReadFile((OpenFileDescriptor *)file, &expected, 4, 0);
        if (!status) status = expected == sum ? 0 : 6;
    }
    return status;
}
#endif
}
