// A fragment of the original drivecheck.cpp: reading the disc through drive errors, and the ROM font
// behind the error messages. Names come from the symbol table; locals are inferred from the code.
#include <dolphin/os/OSFont.h>
#include <dolphin/dvd.h>

class MohLoaderGc {
  public:
    static MohLoaderGc *InstanceGc();
    int IsResetConsoleRequested();
    void ResetConsole();
};
int CheckDiscDriveStatus();
void DiscErrorCleared();

int DVDSafeRead(DVDFileInfo *file, void *buffer, int length, int offset) {
    MohLoaderGc *loader = MohLoaderGc::InstanceGc();
    bool flag; // Each loop's own exit test.
    bool failed = false;
    do {
        int state = CheckDiscDriveStatus();
        flag = state == 0;
        failed |= (unsigned int)state > 1;
        if (loader->IsResetConsoleRequested())
            loader->ResetConsole();
    } while (!flag);
    do {
        if (failed) {
            DiscErrorCleared();
            failed = false;
        }
        DVDReadAsyncPrio(file, buffer, length, offset, 0, 2);
        do {
            if (loader->IsResetConsoleRequested())
                loader->ResetConsole();
            int status = DVDGetCommandBlockStatus(&file->cb);
            failed |= (unsigned int)CheckDiscDriveStatus() > 1;
            flag = false;
            if (status == 1)
                flag = failed == false;
        } while (flag);
        do {
            flag = CheckDiscDriveStatus() == 0;
            if (loader->IsResetConsoleRequested())
                loader->ResetConsole();
        } while (!flag);
        if (failed)
            DVDCancelAsync(&file->cb, 0);
    } while (failed);
    return length;
}

// File statics in the original, defined outside this fragment.
extern OSFontHeader *g_pFontData;
extern short g_fontSize;
extern short g_fontSpace;

void InitRomFont() {
    g_pFontData = (OSFontHeader *)new char[OS_FONT_SIZE_ANSI];
    OSInitFont(g_pFontData);
    g_fontSize = g_pFontData->cellWidth * 16;
    g_fontSpace = -16;
}
