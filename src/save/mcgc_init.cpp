// AI-assisted GR8E69 reconstruction; see docs/research/save-popups.md.
#include "MemoryCardGC.h"
#include <stdlib.h>
extern "C" { const unsigned short *LOCALE_getstr(void *, int); void MEM_clear(void *, unsigned int); void MEM_copy(void *, const void *, unsigned int); }
extern char comment1[32] asm("comment1.3245");
extern char comment2[1] asm("comment2.3246");
extern Realmc::IconInfo g_sIconDataInfo;
extern Realmc::BannerInfo g_sBannerDataInfo;
void CMemoryCardGC::Init() {
    char format[32];
    wcstombs(format, (const wchar_t *)LOCALE_getstr(GetMiscTextDatabase(), 12), 32);
    sprintf(comment1, format, -103);
    MEM_clear(&fileInfo, 48);
    fileInfo.prefix.size = GetScratchpadSize();
    fileInfo.prefix.name = Realmc::FILENAME_ALL_FILES;
    fileInfo.prefix.title = comment1; fileInfo.prefix.comment = comment2;
    fileInfo.prefix.titleLength = strlen(comment1); fileInfo.prefix.commentLength = strlen(comment2);
    fileInfo.prefix.icon = &g_sIconDataInfo; fileInfo.prefix.banner = &g_sBannerDataInfo;
    *(const unsigned short **)fileInfo.prefix.unknown_08 = LOCALE_getstr(GetMiscTextDatabase(), 9);
    fileInfo.unknown24 = LOCALE_getstr(GetMiscTextDatabase(), 10);
    fileInfo.unknown28 = LOCALE_getstr(GetMiscTextDatabase(), 11);
    fileInfo.unknown2c = 0;
}
