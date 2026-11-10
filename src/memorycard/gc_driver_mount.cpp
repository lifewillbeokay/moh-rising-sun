// AI-assisted reconstruction of the GameCube memory-card driver's result mapping, probing and mounting; see docs/Memory.md.
#include "RealmcDriver.h"
extern "C" void THREAD_yield(int);
namespace Realmc {
int GCDriver::ConvertCardResult(int result) {
    switch (result) {
    case CARD_RESULT_READY:
        return 0;
    case CARD_RESULT_BUSY:
        return 1;
    case CARD_RESULT_WRONGDEVICE:
        return 2;
    case CARD_RESULT_NOCARD:
        return 3;
    case CARD_RESULT_NOFILE:
        return 4;
    case CARD_RESULT_IOERROR:
        return 5;
    case CARD_RESULT_BROKEN:
        return 6;
    case CARD_RESULT_EXIST:
        return 7;
    case CARD_RESULT_NOENT:
        return 8;
    case CARD_RESULT_INSSPACE:
        return 9;
    case CARD_RESULT_NOPERM:
        return 10;
    case CARD_RESULT_LIMIT:
        return 11;
    case CARD_RESULT_NAMETOOLONG:
        return 12;
    case CARD_RESULT_ENCODING:
        return 13;
    case CARD_RESULT_CANCELED:
        return 14;
    case CARD_RESULT_FATAL_ERROR:
        return 15;
    }
    return 16;
}

GcFileDescriptor *GCDriver::FindFreeFileDescriptor() {
    GcFileDescriptor *file = files;
    for (int i = 0; i < 3; i++) {
        if (file->inUse == 0)
            return file;
        file++;
    }
    return 0;
}

GcFileDescriptor *GCDriver::ConvertFileHandleToDescriptor(OpenFileDescriptor *handle) {
    return (GcFileDescriptor *)handle;
}

int GCDriver::CardExists(const CardID &card) {
    long memSize = 0;
    long sectorSize = 0;
    int result;
    do {
        result = CARDProbeEx(card.slot, &memSize, &sectorSize);
        THREAD_yield(1);
    } while (result == CARD_RESULT_BUSY);
    int status = ConvertCardResult(result);
    if (status == 0 && sectorSize != MEMCARD_SECTOR_SIZE)
        status = 2;
    return status;
}

int GCDriver::GetFileBlocks(const CardID &, const FileInfo *info, unsigned int *headerSize) {
    int size = GetIconSize(info) + 68;
    if (info->banner)
        size += GetBannerSize(info);
    size += 4;
    if (headerSize)
        *headerSize = size;
    size += info->size;
    return (size + sectorSize - 1) / (unsigned int)sectorSize;
}

int GCDriver::GetSectorSize(const CardID &card) {
    int remounted = 0;
    int mountedHere = 0;
    CardID previous = mountedCard;
    if (mMounted) {
        if (card.slot != mountedCard.slot) {
            Unmount(mountedCard);
            remounted = 1;
            Mount(card);
        }
    } else {
        mountedHere = 1;
        Mount(card);
    }
    int result = 0;
    unsigned int size = result;
    unsigned int *sizePointer = &size;
    do {
        result = CARDGetSectorSize(card.slot, sizePointer);
        THREAD_yield(1);
    } while (result == CARD_RESULT_BUSY);
    int status = ConvertCardResult(result);
    if (status != 0) {
        if (status == 3)
            size = MEMCARD_SECTOR_SIZE;
    }
    if (remounted) {
        Unmount(card);
        Mount(previous);
    } else if (mountedHere) {
        Unmount(card);
    }
    return size;
}

void GCDriver::CardRemovalCallback(long, long) {
    mIsCardPresent = 0;
}

int GCDriver::Mount(const CardID &card) {
    mMounted = 0;
    long memSize = 0;
    long sectorSize = 0;
    int result;
    do {
        result = CARDProbeEx(card.slot, &memSize, &sectorSize);
        THREAD_yield(1);
    } while (result == CARD_RESULT_BUSY);
    int status = ConvertCardResult(result);
    if (status != 0)
        return status;
    do {
        result = CARDMount(card.slot, workArea, CardRemovalCallback);
        THREAD_yield(1);
    } while (result == CARD_RESULT_BUSY);
    status = ConvertCardResult(result);
    if (status == 0 || status == 13 || status == 6) {
        do {
            result = CARDCheck(card.slot);
            THREAD_yield(1);
        } while (result == CARD_RESULT_BUSY);
        status = ConvertCardResult(result);
        switch (status) {
        case 0:
        case 6:
        case 13:
            mMounted = 1;
            break;
        }
        if (status != 3)
            mIsCardPresent = 1;
        if (mMounted)
            mountedCard = card;
    }
    return status;
}

int GCDriver::Unmount(const CardID &card) {
    if (mMounted) {
        mMounted = 0;
        mIsCardPresent = 0;
        int result;
        do {
            result = CARDUnmount(card.slot);
            THREAD_yield(1);
        } while (result == CARD_RESULT_BUSY);
        return ConvertCardResult(result);
    }
    return 0;
}
}
