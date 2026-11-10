// AI-assisted reconstruction of the GameCube memory-card driver's file header and buffered writes; see docs/Memory.md.
#include "RealmcDriver.h"
extern "C" void THREAD_yield(int);
extern "C" void *memset(void *, int, unsigned int);
extern "C" void *memcpy(void *, const void *, unsigned int);
extern "C" void MEM_clear(void *, int);
namespace Realmc {
int GCDriver::WriteHeaderData(GcFileDescriptor *file) {
    FileInfo *info = &file->info;
    int result = 0;
    if (WriteFile((OpenFileDescriptor *)file, &info->size, 4, 0) == 15)
        result = 15;
    int size = info->size;
    int sum = 0;
    char buffer[32];
    unsigned int i = 0;
    char *p = (char *)&size;
    for (; i < 4; i++)
        sum += *p++;
    int length = info->titleLength;
    if (length > 32)
        length = 32;
    memset(buffer, 0, 32);
    memcpy(buffer, info->title, length);
    if (WriteFile((OpenFileDescriptor *)file, buffer, 32, 0) == 15)
        result = 15;
    i = 0;
    p = buffer;
    for (; i < 32; i++)
        sum += *p++;
    length = info->commentLength;
    if (length > 32)
        length = 32;
    memset(buffer, 0, 32);
    memcpy(buffer, info->comment, length);
    if (WriteFile((OpenFileDescriptor *)file, buffer, 32, 0) == 15)
        result = 15;
    p = buffer;
    for (i = 0; i < 32; i++)
        sum += *p++;
    if (info->banner) {
        unsigned int bytes = GetBannerSize(info);
        if (WriteFile((OpenFileDescriptor *)file, info->banner->data, bytes, 0) == 15)
            result = 15;
        p = info->banner->data;
        for (i = 0; i < bytes; i++)
            sum += *p++;
    }
    if (info->icon) {
        unsigned int bytes = GetIconSize(info);
        if (WriteFile((OpenFileDescriptor *)file, info->icon->data, bytes, 0) == 15)
            result = 15;
        p = info->icon->data;
        for (i = 0; i < bytes; i++)
            sum += *p++;
    }
    if (WriteFile((OpenFileDescriptor *)file, &sum, 4, 0) == 15)
        result = 15;
    return result;
}

int GCDriver::GetBannerSize(const FileInfo *info) {
    int size = 6144;
    if (info->banner->wide)
        size = 3584;
    return size;
}

int GCDriver::GetIconSize(const FileInfo *info) {
    IconInfo *icon = info->icon;
    if (!icon->compact)
        return icon->count * 2048;
    return icon->count * 1536;
}
}
