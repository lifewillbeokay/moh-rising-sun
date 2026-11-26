// AI-assisted reconstruction; see docs/research/card-tasks.md.
#include "RealmcDriver.h"
extern "C" void THREAD_yield(int);
extern "C" void MEM_clear(void *, int);
namespace Realmc {
#ifdef DECOMP_COMPARE
int GCDriver::WriteFile(OpenFileDescriptor *handle, void *data, int count, int *written) {
    GcFileDescriptor *file = ConvertFileHandleToDescriptor(handle);
    if (!file->onCard) return NGCSportsBioWrite(file, data, count, written);
    if (written) *written = count;
    unsigned char *input = (unsigned char *)data;
    while (count > 0) {
        while (file->position < sectorSize && count > 0) {
            ((unsigned char *)buffer)[file->position++] = *input++;
            count--;
            file->dirty = 1;
        }
        if (file->position == sectorSize) {
            if (file->offset < 0) break;
            int result;
            do {
                result = CARDWrite(&file->cardFile, buffer, sectorSize, file->offset);
                THREAD_yield(1);
            } while (result == CARD_RESULT_BUSY);
            int status = ConvertCardResult(result);
            if (status) return status;
            file->dirty = 0;
            file->position = 0;
            file->offset += sectorSize;
            unsigned int capacity = GetFileBlocks(file->card, &file->info, 0) * sectorSize;
            // Preserve the original comparison, including its apparently unusual direction.
            if (capacity < (unsigned int)file->offset) {
                do {
                    result = CARDRead(&file->cardFile, buffer, sectorSize, file->offset);
                    THREAD_yield(1);
                } while (result == CARD_RESULT_BUSY);
                status = ConvertCardResult(result);
                if (status) return status;
                file->offset += sectorSize;
            } else MEM_clear(buffer, sectorSize);
        }
    }
    return 0;
}
#endif
}
