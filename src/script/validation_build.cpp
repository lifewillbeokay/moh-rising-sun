// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include <stdio.h>
#include <string.h>

extern "C" int FILESYS_opensync(const char *, int, int, int *);
extern "C" int FILESYS_sizesync(int, int);
extern "C" int FILESYS_writesync(int, int, const void *, int, int);
extern "C" int FILESYS_closesync(int, int);

void BIFunc_ValidationBuild(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int stage = args[1];
    char text[56];
    if (stage < 0)
        sprintf(text, "ValidationFile.txt");
    else
        sprintf(text, "ValidationLog.txt");
    int handle = -1;
    if (FILESYS_opensync(text, 2, 100, &handle)) {
        sprintf(text, "Validation stage:%d\n", stage);
        int offset = FILESYS_sizesync(handle, 100);
        FILESYS_writesync(handle, offset, text, strlen(text), 100);
        FILESYS_closesync(handle, 100);
    }
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
