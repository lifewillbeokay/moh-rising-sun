// AI-assisted reconstruction of the GameCube memory-card driver's write flush, status, reads and file management; see docs/Memory.md.
#include "RealmcDriver.h"
extern "C" void THREAD_yield(int);
extern "C" void *memset(void *, int, unsigned int);
namespace Realmc {
int GCDriver::FlushWriteBuffer(OpenFileDescriptor *handle) {
    int status = 0;
    GcFileDescriptor *file = ConvertFileHandleToDescriptor(handle);
    if (file->position > 0) {
        int end = file->headerSize + file->info.size;
        if (end <= file->offset - sectorSize + file->position) {
            file->position = end % (unsigned int)sectorSize;
            while (file->position < sectorSize) {
                ((char *)buffer)[file->position] = 0;
                file->position++;
            }
        }
        int result;
        do {
            result = CARDWrite(&file->cardFile, buffer, sectorSize, file->offset);
            THREAD_yield(1);
        } while (result == CARD_RESULT_BUSY);
        status = ConvertCardResult(result);
    }
    file->dirty = 0;
    return status;
}

int GCDriver::SetHeaderInfo(GcFileDescriptor *file) {
    memset(&file->stat, 0, sizeof(file->stat));
    file->stat.commentAddr = 4;
    file->stat.iconAddr = 68;
    if (file->info.banner) {
        if (!file->info.banner->wide)
            CARDSetBannerFormat(&file->stat, 2);
        else
            CARDSetBannerFormat(&file->stat, 1);
    } else {
        CARDSetBannerFormat(&file->stat, 0);
    }
    int count = file->info.icon->count;
    if (count < 8) {
        CARDSetIconFormat(&file->stat, count, 0);
        CARDSetIconSpeed(&file->stat, count, 0);
    }
    if (file->info.icon->animation == 2)
        CARDSetIconAnim(&file->stat, 4);
    else if (file->info.icon->animation == 1)
        CARDSetIconAnim(&file->stat, 0);
    int i = 0;
    if (i < count) {
        IconInfo *icon = file->info.icon;
        do {
            if (!icon->compact)
                CARDSetIconFormat(&file->stat, i, 2);
            else
                CARDSetIconFormat(&file->stat, i, 1);
            switch (file->info.icon->speeds[i]) {
            case 1:
                CARDSetIconSpeed(&file->stat, i, 3);
                break;
            case 2:
                CARDSetIconSpeed(&file->stat, i, 2);
                break;
            case 3:
                CARDSetIconSpeed(&file->stat, i, 1);
                break;
            default:
                CARDSetIconSpeed(&file->stat, i, 2);
                break;
            }
            i++;
        } while (i < count);
    }
    int result;
    do {
        result = CARDSetStatus(file->card.slot, file->fileNo, &file->stat);
        THREAD_yield(1);
    } while (result == CARD_RESULT_BUSY);
    int status = ConvertCardResult(result);
    if (status != 0)
        return -1;
    return 0;
}

int GCDriver::ReadFile(OpenFileDescriptor *handle, void *data, int size, int *read) {
    GcFileDescriptor *file = ConvertFileHandleToDescriptor(handle);
    if (!file->onCard)
        return NGCSportsBioRead(file, data, size, read);
    char *target = (char *)data;
    int status = 0;
    int remaining = size;
    if (file->dirty)
        FlushWriteBuffer(handle);
    int limit = (int)file->stat.length;
    int base;
    if (file->offset)
        base = file->offset - sectorSize;
    else
        base = 0;
    while (remaining) {
        int position = base + file->position;
        if (position < 0 || position >= limit) {
            status = 11;
            break;
        }
        if (file->needsRead) {
            int result;
            do {
                result = CARDRead(&file->cardFile, buffer, sectorSize, file->offset);
                THREAD_yield(1);
            } while (result == CARD_RESULT_BUSY);
            status = ConvertCardResult(result);
            if (status)
                break;
            file->offset += sectorSize;
            file->needsRead = status;
        }
        while (file->position < sectorSize && remaining) {
            if (target) {
                *target = ((char *)buffer)[file->position];
                target++;
            }
            remaining--;
            file->position++;
        }
        if (file->position == sectorSize) {
            file->needsRead = 1;
            file->position = 0;
        }
    }
    if (read)
        *read = size - remaining;
    return status;
}

int GCDriver::CloseFile(OpenFileDescriptor *handle) {
    GcFileDescriptor *file = ConvertFileHandleToDescriptor(handle);
    int result;
    if (!file->onCard) {
        result = NGCSportsBioClose(file);
    } else if (file->openMode & 512) {
        FlushWriteBuffer(handle);
        do {
            result = CARDClose(&file->cardFile);
            THREAD_yield(1);
        } while (result == CARD_RESULT_BUSY);
        result = ConvertCardResult(result);
        if (result == 0) {
            if (SetHeaderInfo(file) == -1)
                result = 15;
        }
    } else {
        do {
            result = CARDClose(&file->cardFile);
            THREAD_yield(1);
        } while (result == CARD_RESULT_BUSY);
        result = ConvertCardResult(result);
    }
    file->inUse = 0;
    return result;
}

int GCDriver::GetOpenFileSize(OpenFileDescriptor *handle) {
    return ConvertFileHandleToDescriptor(handle)->info.size;
}

int GCDriver::GetFreeCardSpace(const CardID &card, int *freeBlocks, int *freeFiles) {
    long bytes = 0;
    long files = 0;
    int result = 15;
    if (CARDFreeBlocks(card.slot, &bytes, &files) == 0)
        result = 0;
    *freeBlocks = bytes / sectorSize;
    *freeFiles = files;
    return result;
}

int GCDriver::DeleteFile(const CardID &card, const FileInfo *info) {
    int fileNo;
    int result;
    do {
        if (card.type == 1) {
            fileNo = -1;
            result = FindFileNumber((char *)info->name, &fileNo);
            if (result == 0)
                result = ConvertCardResult(CARDFastDelete(card.slot, fileNo));
        } else {
            result = ConvertCardResult(CARDDelete(card.slot, info->name));
        }
        THREAD_yield(1);
    } while (result == 1);
    return result;
}

int GCDriver::RenameFile(const CardID &card, const FileInfo *info, const char *name) {
    int result;
    do {
        result = CARDRename(card.slot, info->name, name);
        THREAD_yield(1);
    } while (result == CARD_RESULT_BUSY);
    return ConvertCardResult(result);
}

int GCDriver::FormatCard(const CardID &card) {
    int result;
    do {
        result = CARDFormat(card.slot);
        THREAD_yield(1);
    } while (result == CARD_RESULT_BUSY);
    return ConvertCardResult(result);
}

int GCDriver::IsCardPresent() {
    return mIsCardPresent;
}
}
