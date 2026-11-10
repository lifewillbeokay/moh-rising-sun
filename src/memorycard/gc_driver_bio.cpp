// AI-assisted reconstruction of the GameCube memory-card driver's seeking, attributes and sports BIOS paths; see docs/Memory.md.
#include "RealmcDriver.h"
extern "C" void THREAD_yield(int);
extern "C" int strcmp(const char *, const char *);
namespace Realmc {
int GCDriver::Seek(OpenFileDescriptor *handle, int offset, SeekFrom from) {
    GcFileDescriptor *file = ConvertFileHandleToDescriptor(handle);
    if (!file->onCard)
        return NGCSportsBioSeek(file, offset, from);
    if (file->dirty)
        FlushWriteBuffer(handle);
    switch (from) {
    case SeekFrom_Start:
        offset += file->headerSize;
        break;
    case SeekFrom_Current:
        offset += file->offset - sectorSize + file->position;
        break;
    case SeekFrom_End:
        offset = file->info.size + file->headerSize - offset;
        break;
    }
    if (offset < 0 || offset >= (int)file->stat.length)
        return 11;
    file->offset = offset / sectorSize * sectorSize;
    int position = offset - offset / sectorSize * sectorSize;
    file->needsRead = 1;
    file->position = position;
    return 0;
}

int GCDriver::SetAttributes(CardID card, char *name, FileAttribute attributes) {
    int fileNo = -1;
    int result = FindFileNumber(name, &fileNo);
    if (result == 0) {
        int status;
        do {
            status = CARDSetAttributes(card.slot, fileNo, attributes);
        } while (status == CARD_RESULT_BUSY);
        result = ConvertCardResult(status);
    }
    return result;
}

int GCDriver::FindFileNumber(char *name, int *fileNo) {
    *fileNo = -1;
    int i = 0;
    do {
        char stat[108];
        int result;
        do {
            result = CARDGetStatus(0, i, stat);
            THREAD_yield(1);
        } while (result == CARD_RESULT_BUSY);
        int status = ConvertCardResult(result);
        switch (status) {
        case 0:
            if (strcmp(stat, name) == 0) {
                *fileNo = i;
                return 0;
            }
            break;
        case 4:
        case 10:
            break;
        default:
            return status;
        }
        i++;
    } while (i < 127);
    return 4;
}

int GCDriver::NGCSportsBioSeek(GcFileDescriptor *file, int offset, SeekFrom from) {
    switch (from) {
    case SeekFrom_Start:
        file->offset = offset;
        break;
    case SeekFrom_Current:
        file->offset += offset;
        break;
    }
    if (file->offset < 0 || file->offset >= (int)file->stat.length)
        return 11;
    return 0;
}

int GCDriver::NGCSportsBioRead(GcFileDescriptor *file, void *data, int size, int *read) {
    int result;
    do {
        result = CARDRead(&file->cardFile, data, size, file->offset);
        THREAD_yield(1);
    } while (result == CARD_RESULT_BUSY);
    int status;
    switch (ConvertCardResult(result)) {
    case 0:
        *read = size;
        status = 0;
        file->offset += size;
        break;
    case 11:
        *read = 0;
        status = 0;
        break;
    default:
        *read = size;
        status = 15;
        break;
    }
    return status;
}

int GCDriver::NGCSportsBioWrite(GcFileDescriptor *file, void *data, int size, int *written) {
    int result;
    do {
        result = CARDWrite(&file->cardFile, data, size, file->offset);
        THREAD_yield(1);
    } while (result == CARD_RESULT_BUSY);
    int status = ConvertCardResult(result);
    switch (status) {
    case 0:
        *written = size;
        file->offset += size;
        break;
    case 11:
        *written = 0;
        status = 0;
        break;
    case 10:
        *written = -1;
        status = 0;
        break;
    case 15:
    default:
        *written = 0;
        break;
    }
    return status;
}

int GCDriver::NGCSportsBioClose(GcFileDescriptor *file) {
    int result;
    do {
        result = CARDClose(&file->cardFile);
        THREAD_yield(1);
    } while (result == CARD_RESULT_BUSY);
    int status = ConvertCardResult(result);
    if (status == 0)
        file->inUse = 0;
    return status;
}
}
